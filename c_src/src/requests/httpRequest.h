/**
 * @file httpRequest.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_HTTP_REQUEST_H
#define CBERL_HTTP_REQUEST_H

#include <libcouchbase/couchbase.h>

#include <string>
#include <tuple>

namespace cb {

class HttpRequest {
public:
    using Raw = std::tuple<std::int32_t, std::int32_t, std::string, std::string,
        std::string>;

    HttpRequest(Raw raw);

    lcb_http_type_t type() const;

    lcb_http_method_t method() const;

    const std::string &path() const;

    const std::string &contentType() const;

    const std::string &body() const;

private:
    lcb_http_type_t m_type;
    lcb_http_method_t m_method;
    std::string m_path;
    std::string m_contentType;
    std::string m_body;
};

} // namespace cb

#endif // CBERL_HTTP_REQUEST_H
