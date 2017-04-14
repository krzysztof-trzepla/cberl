/**
 * @file httpResponse.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_HTTP_RESPONSE_H
#define CBERL_HTTP_RESPONSE_H

#include "response.h"

namespace cb {

class HttpResponse : public Response {
public:
    HttpResponse(lcb_error_t err);

    void setError(lcb_error_t err);

    void setStatus(lcb_http_status_t status);

    void setBody(const void *body, std::size_t bodySize);

    nifpp::TERM toTerm(const Env &env) const;

private:
    lcb_http_status_t m_status;
    std::string m_body;
};

} // namespace cb

#endif // CBERL_HTTP_RESPONSE_H
