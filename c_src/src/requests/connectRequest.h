/**
 * @file connectRequest.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_CONNECT_REQUEST_H
#define CBERL_CONNECT_REQUEST_H

#include "nifpp.h"

#include <libcouchbase/couchbase.h>

#include <string>
#include <tuple>
#include <vector>

namespace cb {

class ConnectRequest {
public:
    ConnectRequest(std::string host, std::string username, std::string password,
        std::string bucket,
        std::vector<std::tuple<nifpp::str_atom, int>> options);

    const std::string &host() const;

    const std::string &username() const;

    const std::string &password() const;

    const std::string &bucket() const;

    const std::vector<std::tuple<nifpp::str_atom, int>> &options() const;

private:
    std::string m_host;
    std::string m_username;
    std::string m_password;
    std::string m_bucket;
    std::vector<std::tuple<nifpp::str_atom, int>> m_options;
};

} // namespace cb

#endif // CBERL_CONNECT_REQUEST_H
