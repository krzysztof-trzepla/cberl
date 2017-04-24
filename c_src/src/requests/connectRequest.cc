/**
 * @file connectRequest.cc
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#include "connectRequest.h"

namespace cb {

ConnectRequest::ConnectRequest(std::string host, std::string username,
    std::string password, std::string bucket,
    std::vector<std::tuple<nifpp::str_atom, int>> options)
    : m_host{std::move(host)}
    , m_username{std::move(username)}
    , m_password{std::move(password)}
    , m_bucket{std::move(bucket)}
    , m_options{std::move(options)}
{
}

const std::string &ConnectRequest::host() const { return m_host; }

const std::string &ConnectRequest::username() const { return m_username; }

const std::string &ConnectRequest::password() const { return m_password; }

const std::string &ConnectRequest::bucket() const { return m_bucket; }

const std::vector<std::tuple<nifpp::str_atom, int>> &
ConnectRequest::options() const
{
    return m_options;
}

} // namespace cb
