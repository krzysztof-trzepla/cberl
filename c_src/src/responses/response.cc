/**
 * @file response.cc
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#include "response.h"

namespace cb {

Response::Response(lcb_error_t err)
    : m_err{err}
{
}

nifpp::TERM Response::toTerm(const Env &env) const
{
    if (m_err == LCB_SUCCESS) {
        return nifpp::make(env, nifpp::str_atom{"ok"});
    }

    return nifpp::make(env,
        std::make_tuple(
            nifpp::str_atom{"error"}, nifpp::str_atom{errorMessage()}));
}

std::string Response::errorMessage() const
{
    switch (m_err) {
        case LCB_AUTH_CONTINUE:
            return "auth_continue";
        case LCB_AUTH_ERROR:
            return "auth_error";
        case LCB_DELTA_BADVAL:
            return "delta_badval";
        case LCB_E2BIG:
            return "e2big";
        case LCB_EBUSY:
            return "ebusy";
        case LCB_EINTERNAL:
            return "einternal";
        case LCB_EINVAL:
            return "einval";
        case LCB_ENOMEM:
            return "enomem";
        case LCB_ERANGE:
            return "erange";
        case LCB_ERROR:
            return "error";
        case LCB_ETMPFAIL:
            return "etmpfail";
        case LCB_KEY_EEXISTS:
            return "key_eexists";
        case LCB_KEY_ENOENT:
            return "key_enoent";
        case LCB_NETWORK_ERROR:
            return "network_error";
        case LCB_NOT_MY_VBUCKET:
            return "not_my_vbucket";
        case LCB_NOT_STORED:
            return "not_stored";
        case LCB_NOT_SUPPORTED:
            return "not_supported";
        case LCB_UNKNOWN_COMMAND:
            return "unknown_command";
        case LCB_UNKNOWN_HOST:
            return "unknown_host";
        case LCB_PROTOCOL_ERROR:
            return "protocol_error";
        case LCB_ETIMEDOUT:
            return "etimedout";
        case LCB_CONNECT_ERROR:
            return "connect_error";
        case LCB_BUCKET_ENOENT:
            return "bucket_enoent";
        case LCB_CLIENT_ENOMEM:
            return "client_enomem";
        default:
            return "unknown_error";
    }
}

} // namespace cb
