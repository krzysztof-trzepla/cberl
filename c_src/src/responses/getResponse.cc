/**
 * @file getResponse.cc
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#include "getResponse.h"

namespace cb {

GetResponse::GetResponse(lcb_error_t err, const void *key, std::size_t keySize)
    : Response{err}
    , m_key{static_cast<const char *>(key), keySize}
{
}

GetResponse::GetResponse(const void *key, std::size_t keySize, lcb_cas_t cas,
    lcb_uint32_t flags, const void *value, std::size_t valueSize)
    : Response{LCB_SUCCESS}
    , m_key{static_cast<const char *>(key), keySize}
    , m_cas{cas}
    , m_flags{flags}
    , m_value{static_cast<const char *>(value), valueSize}
{
}

nifpp::TERM GetResponse::toTerm(const Env &env) const
{
    if (m_err == LCB_SUCCESS) {
        return nifpp::make(env,
            std::make_tuple(m_key,
                std::make_tuple(
                    nifpp::str_atom{"ok"}, m_cas, m_flags, m_value)));
    }

    return nifpp::make(env, std::make_tuple(m_key, Response::toTerm(env)));
}

} // namespace cb
