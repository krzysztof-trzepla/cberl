/**
 * @file removeResponse.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#include "removeResponse.h"

namespace cb {

RemoveResponse::RemoveResponse(
    lcb_error_t err, const void *key, std::size_t keySize)
    : Response{err}
    , m_key{static_cast<const char *>(key), keySize}
{
}

nifpp::TERM RemoveResponse::toTerm(const Env &env) const
{
    if (m_err == LCB_SUCCESS) {
        return nifpp::make(env, std::make_tuple(m_key, nifpp::str_atom{"ok"}));
    }

    return nifpp::make(env, std::make_tuple(m_key, Response::toTerm(env)));
}

} // namespace cb
