/**
 * @file cberl_nif.cc
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#include "nifpp.h"

#include "client.h"
#include "connection.h"
#include "requests/requests.h"
#include "responses/responses.h"

#include <memory>
#include <random>
#include <string>
#include <vector>

namespace {
class NifCTX {
public:
    NifCTX(ErlNifEnv *env_, const ERL_NIF_TERM argv[])
        : reqPid{nifpp::get<ErlNifPid>(env_, argv[0])}
        , reqId(std::make_tuple(dist(gen), dist(gen), dist(gen)))
    {
    }

    template <typename T> int send(T &&value) const
    {
        return enif_send(nullptr, &reqPid, env,
            nifpp::make(env, std::make_tuple(reqId, std::forward<T>(value))));
    }

    Env env;
    ErlNifPid reqPid;
    std::tuple<int, int, int> reqId;

private:
    static thread_local std::random_device rd;
    static thread_local std::default_random_engine gen;
    static thread_local std::uniform_int_distribution<int> dist;
};

thread_local std::random_device NifCTX::rd{};
thread_local std::default_random_engine NifCTX::gen{NifCTX::rd()};
thread_local std::uniform_int_distribution<int> NifCTX::dist{};
} // namespace

extern "C" {

static int load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info)
{
    return !(nifpp::register_resource<cb::ClientPtr>(env, nullptr, "Client") &&
        nifpp::register_resource<cb::ConnectionPtr>(
            env, nullptr, "Connection"));
}

static int upgrade(
    ErlNifEnv *env, void **priv, void **old_priv_data, ERL_NIF_TERM load_info)
{
    return load(env, priv, load_info);
}

static ERL_NIF_TERM new_nif(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    try {
        auto client = nifpp::construct_resource<cb::ClientPtr>(
            std::make_shared<cb::Client>());
        return nifpp::make(
            env, std::make_tuple(nifpp::str_atom{"ok"}, std::move(client)));
    }
    catch (const nifpp::badarg &) {
        return enif_make_badarg(env);
    }
}

static ERL_NIF_TERM connect_nif(
    ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    try {
        NifCTX ctx{env, argv};

        auto client = nifpp::get<cb::ClientPtr>(env, argv[1]);
        cb::ConnectRequest request{nifpp::get<std::string>(env, argv[2]),
            nifpp::get<std::string>(env, argv[3]),
            nifpp::get<std::string>(env, argv[4]),
            nifpp::get<std::string>(env, argv[5])};

        client->connect(
            std::move(request), [ctx](const cb::ConnectResponse &response) {
                ctx.send(response.toTerm(ctx.env));
            });

        return nifpp::make(
            env, std::make_tuple(nifpp::str_atom{"ok"}, ctx.reqId));
    }
    catch (const nifpp::badarg &) {
        return enif_make_badarg(env);
    }
}

static ERL_NIF_TERM get_nif(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    try {
        NifCTX ctx{env, argv};

        auto client = nifpp::get<cb::ClientPtr>(env, argv[1]);
        auto connection = nifpp::get<cb::ConnectionPtr>(env, argv[2]);
        cb::MultiRequest<cb::GetRequest> request{
            nifpp::get<std::vector<cb::GetRequest::Raw>>(env, argv[3])};

        client->get(std::move(connection), std::move(request),
            [ctx](const cb::MultiResponse<cb::GetResponse> &responses) {
                ctx.send(responses.toTerm(ctx.env));
            });

        return nifpp::make(
            env, std::make_tuple(nifpp::str_atom{"ok"}, ctx.reqId));
    }
    catch (const nifpp::badarg &) {
        return enif_make_badarg(env);
    }
}

static ERL_NIF_TERM store_nif(
    ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    try {
        NifCTX ctx{env, argv};

        auto client = nifpp::get<cb::ClientPtr>(env, argv[1]);
        auto connection = nifpp::get<cb::ConnectionPtr>(env, argv[2]);
        cb::MultiRequest<cb::StoreRequest> request{
            nifpp::get<std::vector<cb::StoreRequest::Raw>>(env, argv[3])};

        client->store(std::move(connection), std::move(request),
            [ctx](const cb::MultiResponse<cb::StoreResponse> &responses) {
                ctx.send(responses.toTerm(ctx.env));
            });

        return nifpp::make(
            env, std::make_tuple(nifpp::str_atom{"ok"}, ctx.reqId));
    }
    catch (const nifpp::badarg &) {
        return enif_make_badarg(env);
    }
}

static ERL_NIF_TERM remove_nif(
    ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    try {
        NifCTX ctx{env, argv};

        auto client = nifpp::get<cb::ClientPtr>(env, argv[1]);
        auto connection = nifpp::get<cb::ConnectionPtr>(env, argv[2]);
        cb::MultiRequest<cb::RemoveRequest> request{
            nifpp::get<std::vector<cb::RemoveRequest::Raw>>(env, argv[3])};

        client->remove(std::move(connection), std::move(request),
            [ctx](const cb::MultiResponse<cb::RemoveResponse> &responses) {
                ctx.send(responses.toTerm(ctx.env));
            });

        return nifpp::make(
            env, std::make_tuple(nifpp::str_atom{"ok"}, ctx.reqId));
    }
    catch (const nifpp::badarg &) {
        return enif_make_badarg(env);
    }
}

static ERL_NIF_TERM arithmetic_nif(
    ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    try {
        NifCTX ctx{env, argv};

        auto client = nifpp::get<cb::ClientPtr>(env, argv[1]);
        auto connection = nifpp::get<cb::ConnectionPtr>(env, argv[2]);
        cb::MultiRequest<cb::ArithmeticRequest> request{
            nifpp::get<std::vector<cb::ArithmeticRequest::Raw>>(env, argv[3])};

        client->arithmetic(std::move(connection), std::move(request),
            [ctx](const cb::MultiResponse<cb::ArithmeticResponse> &responses) {
                ctx.send(responses.toTerm(ctx.env));
            });

        return nifpp::make(
            env, std::make_tuple(nifpp::str_atom{"ok"}, ctx.reqId));
    }
    catch (const nifpp::badarg &) {
        return enif_make_badarg(env);
    }
}

static ERL_NIF_TERM http_nif(
    ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    try {
        NifCTX ctx{env, argv};

        auto client = nifpp::get<cb::ClientPtr>(env, argv[1]);
        auto connection = nifpp::get<cb::ConnectionPtr>(env, argv[2]);
        cb::HttpRequest request{nifpp::get<cb::HttpRequest::Raw>(env, argv[3])};

        client->http(std::move(connection), std::move(request),
            [ctx](const cb::HttpResponse &responses) {
                ctx.send(responses.toTerm(ctx.env));
            });

        return nifpp::make(
            env, std::make_tuple(nifpp::str_atom{"ok"}, ctx.reqId));
    }
    catch (const nifpp::badarg &) {
        return enif_make_badarg(env);
    }
}

static ERL_NIF_TERM durability_nif(
    ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    try {
        NifCTX ctx{env, argv};

        auto client = nifpp::get<cb::ClientPtr>(env, argv[1]);
        auto connection = nifpp::get<cb::ConnectionPtr>(env, argv[2]);
        cb::MultiRequest<cb::DurabilityRequest> request{
            nifpp::get<std::vector<cb::DurabilityRequest::Raw>>(env, argv[3])};
        cb::DurabilityRequestOptions options{
            nifpp::get<cb::DurabilityRequestOptions::Raw>(env, argv[4])};

        client->durability(std::move(connection), std::move(request),
            std::move(options),
            [ctx](const cb::MultiResponse<cb::DurabilityResponse> &responses) {
                ctx.send(responses.toTerm(ctx.env));
            });

        return nifpp::make(
            env, std::make_tuple(nifpp::str_atom{"ok"}, ctx.reqId));
    }
    catch (const nifpp::badarg &) {
        return enif_make_badarg(env);
    }
}

static ErlNifFunc nif_funcs[] = {{"new", 0, new_nif},
    {"connect", 6, connect_nif}, {"get", 4, get_nif}, {"store", 4, store_nif},
    {"remove", 4, remove_nif}, {"arithmetic", 4, arithmetic_nif},
    {"http", 4, http_nif}, {"durability", 5, durability_nif}};

ERL_NIF_INIT(cberl_nif, nif_funcs, load, NULL, upgrade, NULL)
}
