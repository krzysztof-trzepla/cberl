/**
 * @file storeResponse.cc
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#include "storeResponse.h"

namespace cb {

StoreResponse::StoreResponse(
    lcb_error_t err, const void *key, std::size_t keySize)
    : Response{err}
    , m_key{static_cast<const char *>(key), keySize}
{
}

StoreResponse::StoreResponse(
    const void *key, std::size_t keySize, lcb_cas_t cas)
    : Response{LCB_SUCCESS}
    , m_key{static_cast<const char *>(key), keySize}
    , m_cas{cas}
{
}

nifpp::TERM StoreResponse::toTerm(const Env &env) const
{
    if (m_err == LCB_SUCCESS) {
        return nifpp::make(env,
            std::make_tuple(
                m_key, std::make_tuple(nifpp::str_atom{"ok"}, m_cas)));
    }

    return nifpp::make(env, std::make_tuple(m_key, Response::toTerm(env)));
}

} // namespace cb
