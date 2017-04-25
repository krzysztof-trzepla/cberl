%%%-------------------------------------------------------------------
%%% @author Krzysztof Trzepla
%%% @copyright (C) 2017: Krzysztof Trzepla
%%% This software is released under the MIT license cited in 'LICENSE.md'.
%%% @end
%%%-------------------------------------------------------------------
%%% @doc
%%% This module provides API to access CouchBase database.
%%% @end
%%%-------------------------------------------------------------------
-module(cberl).
-author("Krzysztof Trzepla").

-behaviour(gen_server).

%% API
-export([connect/6, get/5, bulk_get/3, store/8, bulk_store/3, remove/4,
    bulk_remove/3, arithmetic/6, bulk_arithmetic/3, http/7, durability/6,
    bulk_durability/4]).

%% gen_server callbacks
-export([init/1, handle_call/3, handle_cast/2, handle_info/2, terminate/2,
    code_change/3]).

-type connection() :: pid().
-type host() :: binary().
-type username() :: binary().
-type password() :: binary().
-type bucket() :: binary().
-type connect_opt() :: {operation_timeout, pos_integer()} | % in microseconds
                       {config_total_timeout, pos_integer()} | % in microseconds
                       {view_timeout, pos_integer()} | % in microseconds
                       {durability_timeout, pos_integer()} | % in microseconds
                       {http_timeout, pos_integer()}. % in microseconds
-type key() :: binary().
-type value() :: binary() | jiffy:json_value() | term().
-type encoder() :: none | json | raw.
-type cas() :: non_neg_integer().
-type expiry() :: non_neg_integer().
-type store_operation() :: add | replace | set | append | prepend.
-type arithmetic_delta() :: integer().
-type arithmetic_default() :: undefined | non_neg_integer().
-type http_type() :: view | management | raw.
-type http_method() :: get | post | put | delete.
-type http_path() :: binary().
-type http_content_type() :: binary().
-type http_status() :: integer().
-type http_body() :: binary().
-type persist_to() :: -1 | non_neg_integer().
-type replicate_to() :: -1 | non_neg_integer().

-export_type([connection/0, host/0, username/0, password/0, bucket/0,
    connect_opt/0]).
-export_type([key/0, value/0, encoder/0, cas/0, expiry/0]).
-export_type([store_operation/0]).
-export_type([arithmetic_delta/0, arithmetic_default/0]).
-export_type([http_type/0, http_method/0, http_path/0, http_content_type/0,
    http_status/0, http_body/0]).
-export_type([persist_to/0, replicate_to/0]).

-type get_request() :: {key(), expiry(), boolean()}.
-type get_response() :: {key(), {ok, cas(), value()} | {error, term()}}.
-type store_request() :: {store_operation(), key(), value(), encoder(), cas(),
                          expiry()}.
-type store_response() :: {key(), {ok, cas()} | {error, term()}}.
-type remove_request() :: {key(), cas()}.
-type remove_response() :: {key(), ok | {error, term()}}.
-type arithmetic_request() :: {key(), arithmetic_delta(), arithmetic_default(),
                               expiry()}.
-type arithmetic_response() :: {key(), {ok, cas(), non_neg_integer()} |
                               {error, term()}}.
-type http_response() :: {ok, http_status(), http_body()} | {error, term()}.
-type durability_request() :: {key(), cas()}.
-type durability_response() :: {key(), {ok, cas()} | {error, term()}}.
-type durability_options() :: {persist_to(), replicate_to()}.

-export_type([get_request/0, get_response/0, store_request/0, store_response/0,
    remove_request/0, remove_response/0, arithmetic_request/0,
    arithmetic_response/0, durability_request/0, durability_response/0,
    durability_options/0]).

-record(state, {
    client :: cberl_nif:client(),
    connection :: cberl_nif:connection()
}).

-type state() :: #state{}.

%%%===================================================================
%%% API
%%%===================================================================

%%--------------------------------------------------------------------
%% @doc
%% Creates connection to a CouchBase database.
%% @end
%%--------------------------------------------------------------------
-spec connect(host(), username(), password(), bucket(), [connect_opt()],
    timeout()) -> {ok, connection()} | no_return().
connect(Host, Username, Password, Bucket, Opts, Timeout) ->
    gen_server:start_link(?MODULE, [
        Host, Username, Password, Bucket, Opts, Timeout
    ], []).

%%--------------------------------------------------------------------
%% @doc
%% Returns value from a CouchBase database.
%% @end
%%--------------------------------------------------------------------
-spec get(connection(), key(), expiry(), boolean(), timeout()) ->
    {ok, cas(), value()} | {error, Reason :: term()}.
get(Connection, Key, Expiry, Lock, Timeout) ->
    Requests = [{Key, Expiry, Lock}],
    case bulk_get(Connection, Requests, Timeout) of
        {ok, [{Key, {ok, Cas, Value}}]} -> {ok, Cas, Value};
        {ok, [{Key, {error, Reason}}]} -> {error, Reason};
        {error, Reason} -> {error, Reason}
    end.

%%--------------------------------------------------------------------
%% @doc
%% Returns values from a CouchBase database using bulk request.
%% @end
%%--------------------------------------------------------------------
-spec bulk_get(connection(), [get_request()], timeout()) ->
    {ok, [get_response()]} | {error, Reason :: term()}.
bulk_get(Connection, Requests, Timeout) ->
    case call(Connection, {get, [Requests]}, Timeout) of
        {ok, Responses} ->
            Responses2 = lists:map(fun
                ({Key, {ok, Cas, Flags, Value}}) ->
                    {Key, {ok, Cas, decode(Flags, Value)}};
                ({Key, {error, Reason}}) ->
                    {Key, {error, Reason}}
            end, Responses),
            {ok, Responses2};
        {error, Reason} ->
            {error, Reason}
    end.

%%--------------------------------------------------------------------
%% @doc
%% Stores key-value pair in a CouchBase database.
%% @end
%%--------------------------------------------------------------------
-spec store(connection(), store_operation(), key(), value(), encoder(), cas(),
    expiry(), timeout()) -> {ok, cas()} | {error, Reason :: term()}.
store(Connection, Operation, Key, Value, Encoder, Cas, Expiry, Timeout) ->
    Requests = [{Operation, Key, Value, Encoder, Cas, Expiry}],
    case bulk_store(Connection, Requests, Timeout) of
        {ok, [{Key, {ok, Cas2}}]} -> {ok, Cas2};
        {ok, [{Key, {error, Reason}}]} -> {error, Reason};
        {error, Reason} -> {error, Reason}
    end.

%%--------------------------------------------------------------------
%% @doc
%% Stores key-value pairs in a CouchBase database using bulk request.
%% @end
%%--------------------------------------------------------------------
-spec bulk_store(connection(), [store_request()], timeout()) ->
    {ok, [store_response()]} | {error, Reason :: term()}.
bulk_store(Connection, Requests, Timeout) ->
    Requests2 = lists:map(fun({Operation, Key, Value, Encoder, Cas, Expiry}) ->
        OperationId = get_store_operation_id(Operation),
        {EncoderId, Value2} = encode(Encoder, Value),
        {OperationId, Key, Value2, EncoderId, Cas, Expiry}
    end, Requests),
    call(Connection, {store, [Requests2]}, Timeout).

%%--------------------------------------------------------------------
%% @doc
%% Removes key-value pair from a CouchBase database.
%% @end
%%--------------------------------------------------------------------
-spec remove(connection(), key(), cas(), timeout()) ->
    ok | {error, Reason :: term()}.
remove(Connection, Key, Cas, Timeout) ->
    Requests = [{Key, Cas}],
    case bulk_remove(Connection, Requests, Timeout) of
        {ok, [{Key, ok}]} -> ok;
        {ok, [{Key, {error, Reason}}]} -> {error, Reason};
        {error, Reason} -> {error, Reason}
    end.

%%--------------------------------------------------------------------
%% @doc
%% Removes key-value pairs from a CouchBase database using bulk request.
%% @end
%%--------------------------------------------------------------------
-spec bulk_remove(connection(), [remove_request()], timeout()) ->
    {ok, [remove_response()]} | {error, Reason :: term()}.
bulk_remove(Connection, Requests, Timeout) ->
    call(Connection, {remove, [Requests]}, Timeout).

%%--------------------------------------------------------------------
%% @doc
%% Performs arithmetic operation in a CouchBase database.
%% @end
%%--------------------------------------------------------------------
-spec arithmetic(connection(), key(), arithmetic_delta(), arithmetic_default(),
    expiry(), timeout()) -> {ok, cas(), value()} | {error, Reason :: term()}.
arithmetic(Connection, Key, Delta, Default, Expiry, Timeout) ->
    Requests = [{Key, Delta, Default, Expiry}],
    case bulk_arithmetic(Connection, Requests, Timeout) of
        {ok, [{Key, {ok, Cas, Value}}]} -> {ok, Cas, Value};
        {ok, [{Key, {error, Reason}}]} -> {error, Reason};
        {error, Reason} -> {error, Reason}
    end.

%%--------------------------------------------------------------------
%% @doc
%% Performs arithmetic operations in a CouchBase database using bulk request.
%% @end
%%--------------------------------------------------------------------
-spec bulk_arithmetic(connection(), [arithmetic_request()], timeout()) ->
    {ok, [arithmetic_response()]} | {error, Reason :: term()}.
bulk_arithmetic(Connection, Requests, Timeout) ->
    Requests2 = lists:map(fun({Key, Delta, Default, Expiry}) ->
        {Create, Initial} = case Default of
            undefined -> {false, 0};
            _ -> {true, Default}
        end,
        {Key, Delta, Create, Initial, Expiry}
    end, Requests),
    call(Connection, {arithmetic, [Requests2]}, Timeout).

%%--------------------------------------------------------------------
%% @doc
%% Performs HTTP request to a CouchBase database.
%% @end
%%--------------------------------------------------------------------
-spec http(connection(), http_type(), http_method(), http_path(),
    http_content_type(), http_body(), timeout()) -> http_response().
http(Connection, Type, Method, Path, ContentType, Body, Timeout) ->
    TypeId = get_http_type_id(Type),
    MethodId = get_http_method_id(Method),
    Request = {TypeId, MethodId, Path, ContentType, Body},
    call(Connection, {http, [Request]}, Timeout).

%%--------------------------------------------------------------------
%% @doc
%% Performs durability check of a key-value pair in a CouchBase database.
%% @end
%%--------------------------------------------------------------------
-spec durability(connection(), key(), cas(), persist_to(), replicate_to(),
    timeout()) -> {ok, cas()} | {error, Reason :: term()}.
durability(Connection, Key, Cas, PersistTo, ReplicateTo, Timeout) ->
    Requests = [{Key, Cas}],
    Options = {PersistTo, ReplicateTo},
    case bulk_durability(Connection, Requests, Options, Timeout) of
        {ok, [{Key, {ok, Cas2}}]} -> {ok, Cas2};
        {ok, [{Key, {error, Reason}}]} -> {error, Reason};
        {error, Reason} -> {error, Reason}
    end.

%%--------------------------------------------------------------------
%% @doc
%% Performs durability check of a key-value pairs in a CouchBase database
%% using bulk request.
%% @end
%%--------------------------------------------------------------------
-spec bulk_durability(connection(), [durability_request()],
    durability_options(), timeout()) ->
    {ok, [durability_response()]} | {error, Reason :: term()}.
bulk_durability(Connection, Requests, Options, Timeout) ->
    call(Connection, {durability, [Requests, Options]}, Timeout).

%%%===================================================================
%%% gen_server callbacks
%%%===================================================================

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Initializes CouchBase connection.
%% @end
%%--------------------------------------------------------------------
-spec init(Args :: term()) ->
    {ok, State :: state()} | {ok, State :: state(), timeout() | hibernate} |
    {stop, Reason :: term()} | ignore.
init([Host, Username, Password, Bucket, Opts, Timeout]) ->
    {ok, Client} = cberl_nif:new(),
    {ok, Ref} = cberl_nif:connect(
        self(), Client, Host, Username, Password, Bucket, Opts
    ),
    receive
        {Ref, {ok, Connection}} ->
            {ok, #state{
                client = Client,
                connection = Connection
            }};
        {Ref, {error, Reason}} ->
            {stop, Reason}
    after
        Timeout -> {stop, timeout}
    end.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Handles call messages.
%% @end
%%--------------------------------------------------------------------
-spec handle_call(Request :: term(), From :: {pid(), Tag :: term()},
    State :: state()) ->
    {reply, Reply :: term(), NewState :: state()} |
    {reply, Reply :: term(), NewState :: state(), timeout() | hibernate} |
    {noreply, NewState :: state()} |
    {noreply, NewState :: state(), timeout() | hibernate} |
    {stop, Reason :: term(), Reply :: term(), NewState :: state()} |
    {stop, Reason :: term(), NewState :: state()}.
handle_call(_Request, _From, #state{} = State) ->
    {noreply, State}.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Handles cast messages.
%% @end
%%--------------------------------------------------------------------
-spec handle_cast(Request :: term(), State :: state()) ->
    {noreply, NewState :: state()} |
    {noreply, NewState :: state(), timeout() | hibernate} |
    {stop, Reason :: term(), NewState :: state()}.
handle_cast({request, Ref, From, {Function, Args}}, #state{} = State) ->
    #state{
        client = Client,
        connection = Connection
    } = State,
    {ok, Ref2} = apply(cberl_nif, Function, [From, Client, Connection | Args]),
    From ! {Ref, {ok, Ref2}},
    {noreply, State};
handle_cast(_Request, #state{} = State) ->
    {noreply, State}.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Handles all non call/cast messages.
%% @end
%%--------------------------------------------------------------------
-spec handle_info(Info :: timeout() | term(), State :: state()) ->
    {noreply, NewState :: state()} |
    {noreply, NewState :: state(), timeout() | hibernate} |
    {stop, Reason :: term(), NewState :: state()}.
handle_info(_Info, #state{} = State) ->
    {noreply, State}.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% This function is called by a gen_server when it is about to
%% terminate. It should be the opposite of Module:init/1 and do any
%% necessary cleaning up. When it returns, the gen_server terminates
%% with Reason. The return value is ignored.
%% @end
%%--------------------------------------------------------------------
-spec terminate(Reason :: (normal | shutdown | {shutdown, term()} | term()),
    State :: state()) -> term().
terminate(_Reason, #state{} = State) ->
    State.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Converts process state when code is changed.
%% @end
%%--------------------------------------------------------------------
-spec code_change(OldVsn :: term() | {down, term()}, State :: state(),
    Extra :: term()) -> {ok, NewState :: state()} | {error, Reason :: term()}.
code_change(_OldVsn, State, _Extra) ->
    {ok, State}.

%%%===================================================================
%%% Internal functions
%%%===================================================================

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Sends request to a CouchBase database and awaits response with timeout.
%% @end
%%--------------------------------------------------------------------
-spec call(connection(), {Function :: atom(), Args :: list()}, timeout()) ->
    cberl_nif:response() | {error, Reason :: term()}.
call(Connection, Request, Timeout) ->
    Ref = make_ref(),
    gen_server:cast(Connection, {request, Ref, self(), Request}),
    case receive_response(Ref, Timeout) of
        {ok, ResponseRef} -> receive_response(ResponseRef, Timeout);
        {error, Reason} -> {error, Reason}
    end.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Waits with a timeout for a response associated with a reference.
%% @end
%%--------------------------------------------------------------------
-spec receive_response(reference() | cberl_nif:request_id(), timeout()) ->
    cberl_nif:response() | {error, Reason :: term()}.
receive_response(Ref, Timeout) ->
    receive
        {Ref, Response} -> Response
    after
        Timeout -> {error, timeout}
    end.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Decodes value based on the encoding flag.
%% @end
%%--------------------------------------------------------------------
-spec decode(cberl_nif:flags(), cberl_nif:value()) -> value().
decode(0, Value) -> Value;
decode(1, Value) -> jiffy:decode(Value);
decode(2, Value) -> binary_to_term(Value).

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Encodes value and returns encoding flag.
%% @end
%%--------------------------------------------------------------------
-spec encode(encoder(), value()) -> cberl_nif:value().
encode(none, Value) -> {0, Value};
encode(json, Value) -> {1, jiffy:encode(Value)};
encode(raw, Value) -> {2, term_to_binary(Value)}.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Converts store operation type to an ID.
%% @end
%%--------------------------------------------------------------------
-spec get_store_operation_id(store_operation()) ->
    cberl_nif:store_operation_id().
get_store_operation_id(add) -> 1;
get_store_operation_id(replace) -> 2;
get_store_operation_id(set) -> 3;
get_store_operation_id(append) -> 4;
get_store_operation_id(prepend) -> 5.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Converts http request type to an ID.
%% @end
%%--------------------------------------------------------------------
-spec get_http_type_id(http_type()) -> cberl_nif:http_type_id().
get_http_type_id(view) -> 0;
get_http_type_id(management) -> 1;
get_http_type_id(raw) -> 2.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Converts http request method to an ID.
%% @end
%%--------------------------------------------------------------------
-spec get_http_method_id(http_method()) -> cberl_nif:http_method_id().
get_http_method_id(get) -> 0;
get_http_method_id(post) -> 1;
get_http_method_id(put) -> 2;
get_http_method_id(delete) -> 3.