/**
 * @file httpRequest.cc
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#include "httpRequest.h"

namespace cb {

HttpRequest::HttpRequest(Raw raw)
    : m_type{static_cast<lcb_http_type_t>(std::get<0>(raw))}
    , m_method{static_cast<lcb_http_method_t>(std::get<1>(raw))}
    , m_path{std::get<2>(raw)}
    , m_contentType{std::get<3>(raw)}
    , m_body{std::get<4>(raw)}
{
}

lcb_http_type_t HttpRequest::type() const { return m_type; }

lcb_http_method_t HttpRequest::method() const { return m_method; }

const std::string &HttpRequest::path() const { return m_path; }

const std::string &HttpRequest::contentType() const { return m_contentType; }

const std::string &HttpRequest::body() const { return m_body; }

} // namespace cb
