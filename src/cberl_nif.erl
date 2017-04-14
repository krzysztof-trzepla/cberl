%%%-------------------------------------------------------------------
%%% @author Krzysztof Trzepla
%%% @copyright (C) 2017: Krzysztof Trzepla
%%% This software is released under the MIT license cited in 'LICENSE.md'.
%%% @end
%%%-------------------------------------------------------------------
%%% @doc
%%% Erlang wrapper for cberl NIF module.
%%% @end
%%%--------------------------------------------------------------------
-module(cberl_nif).
-author("Krzysztof Trzepla").

-on_load(init/0).

%% API
-export([new/0, connect/6, get/4, store/4, remove/4, arithmetic/4, http/4,
    durability/5]).

-type client() :: term().
-type connection() :: term().
-type request_id() :: {integer(), integer(), integer()}.

-export_type([client/0, connection/0, request_id/0]).

-type flags() :: non_neg_integer().
-type value() :: binary().
-type store_operation_id() :: non_neg_integer().
-type http_type_id() :: non_neg_integer().
-type http_method_id() :: non_neg_integer().

-export_type([flags/0, store_operation_id/0, http_type_id/0, http_method_id/0]).

-type get_request() :: cberl:get_request().
-type get_response() :: {cberl:key(),
                           {ok, cberl:cas(), flags(), value()} |
                           {error, term()}
                        }.
-type store_request() :: {store_operation_id(), cberl:key(), value(), flags(),
                          cberl:cas(), cberl:expiry()}.
-type store_response() :: cberl:store_response().
-type remove_request() :: cberl:remove_request().
-type remove_response() :: cberl:remove_response().
-type arithmetic_request() :: {cberl:key(), cberl:arithmetic_delta(), boolean(),
                               cberl:arithmetic_default(), cberl:expiry()}.
-type arithmetic_response() :: cberl:arithmetic_response().
-type http_request() :: {http_type_id(), http_method_id(), cberl:http_path(),
                         cberl:http_content_type(), cberl:http_body()}.
-type http_response() :: cberl:http_response().
-type durability_request() :: cberl:durability_request().
-type durability_response() :: cberl:durability_response().
-type durability_options() :: cberl:durability_options().
-type response() :: get_response() | store_response() | remove_response() |
                    arithmetic_response() | http_response() |
                    durability_response().

-export_type([response/0]).

%%%===================================================================
%%% API
%%%===================================================================

%%--------------------------------------------------------------------
%% @doc
%% Binding for NIF 'new' function.
%% @end
%%--------------------------------------------------------------------
-spec new() -> {ok, client()} | no_return().
new() ->
    erlang:nif_error(cberl_nif_not_loaded).

%%--------------------------------------------------------------------
%% @doc
%% Binding for NIF 'connect' function.
%% @end
%%--------------------------------------------------------------------
-spec connect(pid(), client(), cberl:host(), cberl:username(), cberl:password(),
    cberl:bucket()) -> {ok, request_id()} | no_return().
connect(_From, _Client, _Host, _Username, _Password, _Bucket) ->
    erlang:nif_error(cberl_nif_not_loaded).

%%--------------------------------------------------------------------
%% @doc
%% Binding for NIF 'get' function.
%% @end
%%--------------------------------------------------------------------
-spec get(pid(), client(), connection(), [get_request()]) ->
    {ok, request_id()} | no_return().
get(_From, _Client, _Connection, _Requests) ->
    erlang:nif_error(cberl_nif_not_loaded).

%%--------------------------------------------------------------------
%% @doc
%% Binding for NIF 'store' function.
%% @end
%%--------------------------------------------------------------------
-spec store(pid(), client(), connection(), [store_request()]) ->
    {ok, request_id()} | no_return().
store(_From, _Client, _Connection, _Requests) ->
    erlang:nif_error(cberl_nif_not_loaded).

%%--------------------------------------------------------------------
%% @doc
%% Binding for NIF 'remove' function.
%% @end
%%--------------------------------------------------------------------
-spec remove(pid(), client(), connection(), [remove_request()]) ->
    {ok, request_id()} | no_return().
remove(_From, _Client, _Connection, _Requests) ->
    erlang:nif_error(cberl_nif_not_loaded).

%%--------------------------------------------------------------------
%% @doc
%% Binding for NIF 'arithmetic' function.
%% @end
%%--------------------------------------------------------------------
-spec arithmetic(pid(), client(), connection(), [arithmetic_request()]) ->
    {ok, request_id()} | no_return().
arithmetic(_From, _Client, _Connection, _Requests) ->
    erlang:nif_error(cberl_nif_not_loaded).

%%--------------------------------------------------------------------
%% @doc
%% Binding for NIF 'http' function.
%% @end
%%--------------------------------------------------------------------
-spec http(pid(), client(), connection(), http_request()) ->
    {ok, request_id()} | no_return().
http(_From, _Client, _Connection, _Request) ->
    erlang:nif_error(cberl_nif_not_loaded).

%%--------------------------------------------------------------------
%% @doc
%% Binding for NIF 'durability' function.
%% @end
%%--------------------------------------------------------------------
-spec durability(pid(), client(), connection(), [durability_request()],
    durability_options()) -> {ok, request_id()} | no_return().
durability(_From, _Client, _Connection, _Requests, _Options) ->
    erlang:nif_error(cberl_nif_not_loaded).

%%%===================================================================
%%% Internal functions
%%%===================================================================

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Loads the NIF native library. The library is first searched for
%% in application priv dir, and then under ../priv and ./priv .
%% @end
%%--------------------------------------------------------------------
-spec init() -> ok | {error, Reason :: term()}.
init() ->
    LibName = "libcberl",
    LibPath = case code:priv_dir(cberl) of
        {error, bad_name} ->
            case filelib:is_dir(filename:join(["..", priv])) of
                true ->
                    filename:join(["..", priv, LibName]);
                _ ->
                    filename:join([priv, LibName])
            end;

        Dir ->
            filename:join(Dir, LibName)
    end,

    erlang:load_nif(LibPath, 0).
