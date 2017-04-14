/**
 * @file connectResponse.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_CONNECT_RESPONSE_H
#define CBERL_CONNECT_RESPONSE_H

#include "response.h"
#include "types.h"

namespace cb {

class Connection;

class ConnectResponse : public Response {
public:
    ConnectResponse(lcb_error_t err, ConnectionPtr connection);

    nifpp::TERM toTerm(const Env &env) const;

private:
    std::shared_ptr<Connection> m_connection;
};

} // namespace cb

#endif // CBERL_CONNECT_RESPONSE_H
