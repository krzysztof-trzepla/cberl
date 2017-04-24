%%%-------------------------------------------------------------------
%%% @author Krzysztof Trzepla
%%% @copyright (C) 2017: Krzysztof Trzepla
%%% This software is released under the MIT license cited in 'LICENSE.md'.
%%% @end
%%%-------------------------------------------------------------------
%%% @doc
%%% This file contains cberl tests.
%%% @end
%%%-------------------------------------------------------------------
-module(cberl_test_SUITE).
-author("Krzysztof Trzepla").

-include_lib("common_test/include/ct.hrl").

%% export for ct
-export([all/0, init_per_testcase/2]).

%% tests
-export([
    store_test/1,
    bulk_store_test/1,
    get_test/1,
    bulk_get_test/1,
    remove_test/1,
    bulk_remove_test/1
]).

all() -> [
    store_test,
    bulk_store_test,
    get_test,
    bulk_get_test,
    remove_test,
    bulk_remove_test
].

-define(TIMEOUT, timer:seconds(5)).

%%%===================================================================
%%% Test functions
%%%===================================================================

store_test(Config) ->
    C = ?config(connection, Config),
    lists:foreach(fun({Key, Value, Encoder}) ->
        {ok, _} = cberl:store(C, set, Key, Value, Encoder, 0, 0, ?TIMEOUT)
    end, [
        {<<"k1">>, <<"v1">>, none},
        {<<"k2">>, {[{<<"k2">>, <<"v2">>}]}, json},
        {<<"k3">>, v3, raw}
    ]).

bulk_store_test(Config) ->
    C = ?config(connection, Config),
    {ok, [
        {<<"k1">>, {ok, _}},
        {<<"k2">>, {ok, _}},
        {<<"k3">>, {ok, _}}
    ]} = cberl:bulk_store(C, [
        {set, <<"k1">>, <<"v1">>, none, 0, 0},
        {set, <<"k2">>, {[{<<"k2">>, <<"v2">>}]}, json, 0, 0},
        {set, <<"k3">>, v3, raw, 0, 0}
    ], ?TIMEOUT).

get_test(Config) ->
    C = ?config(connection, Config),
    lists:foreach(fun({Key, Value, Encoder}) ->
        {ok, Cas} = cberl:store(C, set, Key, Value, Encoder, 0, 0, ?TIMEOUT),
        {ok, Cas, Value} = cberl:get(C, Key, 0, false, ?TIMEOUT)
    end, [
        {<<"k1">>, <<"v1">>, none},
        {<<"k2">>, {[{<<"k2">>, <<"v2">>}]}, json},
        {<<"k3">>, v3, raw}
    ]).

bulk_get_test(Config) ->
    C = ?config(connection, Config),
    Value1 = <<"v1">>,
    Value2 = {[{<<"k2">>, <<"v2">>}]},
    Value3 = v3,
    {ok, [
        {<<"k1">>, {ok, Cas1}},
        {<<"k2">>, {ok, Cas2}},
        {<<"k3">>, {ok, Cas3}}
    ]} = cberl:bulk_store(C, [
        {set, <<"k1">>, Value1, none, 0, 0},
        {set, <<"k2">>, Value2, json, 0, 0},
        {set, <<"k3">>, Value3, raw, 0, 0}
    ], ?TIMEOUT),
    {ok, [
        {<<"k1">>, {ok, Cas1, Value1}},
        {<<"k2">>, {ok, Cas2, Value2}},
        {<<"k3">>, {ok, Cas3, Value3}}
    ]} = cberl:bulk_get(C, [
        {<<"k1">>, 0, false},
        {<<"k2">>, 0, false},
        {<<"k3">>, 0, false}
    ], ?TIMEOUT).

remove_test(Config) ->
    C = ?config(connection, Config),
    Key = <<"k">>,
    {ok, _} = cberl:store(C, set, Key, <<"v">>, none, 0, 0, ?TIMEOUT),
    ok = cberl:remove(C, Key, 0, ?TIMEOUT).

bulk_remove_test(Config) ->
    C = ?config(connection, Config),
    {ok, [
        {<<"k1">>, {ok, _}},
        {<<"k2">>, {ok, _}},
        {<<"k3">>, {ok, _}}
    ]} = cberl:bulk_store(C, [
        {set, <<"k1">>, <<"v1">>, none, 0, 0},
        {set, <<"k2">>, {[{<<"k2">>, <<"v2">>}]}, json, 0, 0},
        {set, <<"k3">>, v3, raw, 0, 0}
    ], ?TIMEOUT),
    {ok, [
        {<<"k1">>, ok},
        {<<"k2">>, ok},
        {<<"k3">>, ok}
    ]} = cberl:bulk_remove(C, [
        {<<"k1">>, 0},
        {<<"k2">>, 0},
        {<<"k3">>, 0}
    ], ?TIMEOUT).

%%%===================================================================
%%% Init/teardown functions
%%%===================================================================

init_per_testcase(_Case, Config) ->
    Host = proplists:get_value(host, Config, <<"172.17.0.3">>),
    Username = proplists:get_value(username, Config, <<>>),
    Password = proplists:get_value(password, Config, <<>>),
    Bucket = proplists:get_value(bucket, Config, <<"default">>),
    Opts = [
        {operation_timeout, 5000000},
        {config_total_timeout, 5000000},
        {view_timeout, 60000000},
        {durability_timeout, 30000000},
        {http_timeout, 10000000}
    ],
    {ok, C} = cberl:connect(Host, Username, Password, Bucket, Opts, ?TIMEOUT),
    [{connection, C} | Config].