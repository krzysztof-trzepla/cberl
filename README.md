# cberl

An Erlang client for a CouchBase database.

## Build

Dependencies:

* `cmake` >= 3.1.0
* `erlang` >= 17.0
* `g++` >= 4.9.0 (or `clang`)
* `make`
* `libcouchbase`

Once you have all of the dependencies, simply run `make` in `cberl` directory to
build it.

# User Guide

Add `cberl` as a `rebar` dependency to your project:

```erlang
{deps, [
  {cberl, "1.0.0", {git, "https://github.com/krzysztof-trzepla/cberl.git", {tag, "1.0.0"}}}
}.
```

Now you can use `cberl` as follows:

```erlang
{ok, C} = cberl:connect(<<"127.0.0.1">>, <<>>, <<>>, <<"default">>, 1000).

% Store binary data
cberl:store(C, set, <<"k1">>, <<"v1">>, none, 0, 0, 1000).
% {ok, 1492165487439380480}

% Store JSON data
cberl:store(C, set, <<"k2">>, {[{<<"k2">>, <<"v2">>}]}, json, 0, 0, 1000).
% {ok, 1492165561477824512}

% Store raw Erlang terms
cberl:store(C, set, <<"k3">>, v3, raw, 0, 0, 1000).
% {ok, 1492165654001811456}

% Get data
cberl:get(C, <<"k1">>, 0, false, 1000).
% {ok, 1492165487439380480, <<"v1">>}
cberl:get(C, <<"k2">>, 0, false, 1000).
% {ok, 1492165561477824512, {[{<<"k2">>, <<"v2">>}]}}
cberl:get(C, <<"k3">>, 0, false, 1000).
% {ok, 1492165654001811456, v3}

% Remove data
cberl:remove(C, <<"k1">>, 0, 1000).
% ok

% Bulk store data
cberl:bulk_store(C, [
    {set, <<"k1">>, <<"v1">>, none, 0, 0},
    {set, <<"k2">>, {[{<<"k2">>, <<"v2">>}]}, json, 0, 0}, 
    {set, <<"k3">>, v3, raw, 0, 0}
], 1000).
% {ok, [{<<"k1">>, {ok, 1492166534118965248}},
%       {<<"k2">>, {ok, 1492166534119227392}},
%       {<<"k3">>, {ok, 1492166534119358464}}]}

% Bulk get data
cberl:bulk_get(C, [
    {<<"k1">>, 0, false}, 
    {<<"k2">>, 0, false},
    {<<"k3">>, 0, false}
], 1000).
% {ok, [{<<"k1">>, {ok, 1492166534118965248, <<"v1">>}},
%       {<<"k2">>, {ok, 1492166534119227392, {[{<<"k2">>, <<"v2">>}]}}},
%       {<<"k3">>, {ok, 1492166534119358464, v3}}]}

% Bulk remove data
cberl:bulk_remove(C, [
    {<<"k1">>, 0},
    {<<"k2">>, 0},
    {<<"k3">>, 0}
], 1000).
% {ok, [{<<"k1">>, ok},
%       {<<"k2">>, ok},
%       {<<"k3">>, ok}]}

% Perform arithmetic operation
cberl:arithmetic(C, <<"k4">>, 1, 0, 0, 1000).
% {ok, 1492166979437527040, 0}
cberl:arithmetic(C, <<"k4">>, 1, 0, 0, 1000).
% {ok, 1492166984044969984, 1}

% Perform bulk arithmetic operation
cberl:bulk_arithmetic(C, [
    {<<"k4">>, 2, 0, 0},
    {<<"k5">>, 1, 0, 0}
], 1000).
% {ok, [{<<"k4">>, {ok, 1492167125759885312, 3}},
%       {<<"k5">>, {ok, 1492167125760016384, 0}}]}

% Perform HTTP request
Path = <<"_design/dev_example/_view/all_docs">>.
cberl:http(C2, view, get, Path, <<>>, <<>>, 1000).
% {ok, 200, <<"{
%   \"total_rows\": 2,
%   \"rows\": [\r\n
%       {\"id\": \"k4\", \"key\": \"k4\", \"value\": null},\r\n
%       {\"id\": \"k5\", \"key\": \"k5\", \"value\": null}\r\n
%   ]\r\n
% }\n">>}

% Perform durability check operation
cberl:durability(C, <<"k4">>, 0, 1, -1, 1000).
% {ok, 1492167125759885312}

% Perform bulk durability check operation
cberl:bulk_durability(C2, [
    {<<"k4">>, 0},
    {<<"k5">>, 0}
], {1, -1}, 1000).
% {ok, [{<<"k4">>, {ok, 1492167125759885312}},
%       {<<"k5">>, {ok, 1492167125760016384}}]}
```

## APIs

The following `libcouchbase` functions are currently implemented:

* `lcb_get`
* `lcb_store`
* `lcb_remove`
* `lcb_arithmetic`
* `lcb_make_http_request`
* `lcb_durability_poll`
