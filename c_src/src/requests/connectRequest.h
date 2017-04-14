/**
 * @file connectRequest.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_CONNECT_REQUEST_H
#define CBERL_CONNECT_REQUEST_H

#include <libcouchbase/couchbase.h>

#include <string>
#include <tuple>

namespace cb {

class ConnectRequest {
public:
    ConnectRequest(std::string host, std::string username, std::string password,
        std::string bucket);

    const std::string &host() const;

    const std::string &username() const;

    const std::string &password() const;

    const std::string &bucket() const;

private:
    std::string m_host;
    std::string m_username;
    std::string m_password;
    std::string m_bucket;
};

} // namespace cb

#endif // CBERL_CONNECT_REQUEST_H
