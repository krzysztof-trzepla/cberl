/**
 * @file connectResponse.cc
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#include "connectResponse.h"
#include "connection.h"

namespace cb {

ConnectResponse::ConnectResponse(
    lcb_error_t err, std::shared_ptr<Connection> connection)
    : Response{err}
    , m_connection{std::move(connection)}
{
}

nifpp::TERM ConnectResponse::toTerm(const Env &env) const
{
    if (m_err == LCB_SUCCESS) {
        return nifpp::make(env,
            std::make_tuple(nifpp::str_atom{"ok"},
                nifpp::construct_resource<ConnectionPtr>(m_connection)));
    }

    return Response::toTerm(env);
}

} // namespace cb
