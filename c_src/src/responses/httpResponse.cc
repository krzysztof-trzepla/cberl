/**
 * @file httpResponse.cc
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#include "httpResponse.h"

namespace cb {

HttpResponse::HttpResponse(lcb_error_t err)
    : Response{err}
{
}

void HttpResponse::setError(lcb_error_t err) { m_err = err; }

void HttpResponse::setStatus(lcb_http_status_t status) { m_status = status; }

void HttpResponse::setBody(const void *body, std::size_t bodySize)
{
    if (body) {
        m_body.append(static_cast<const char *>(body), bodySize);
    }
}

nifpp::TERM HttpResponse::toTerm(const Env &env) const
{
    if (m_err == LCB_SUCCESS) {
        return nifpp::make(
            env, std::make_tuple(nifpp::str_atom{"ok"}, m_status, m_body));
    }

    return Response::toTerm(env);
}

} // namespace cb
