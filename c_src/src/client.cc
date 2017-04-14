/**
 * @file client.cc
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#include "client.h"
#include "connection.h"

#include <asio/post.hpp>

namespace cb {

Client::Client()
    : m_ioService{1}
    , m_work{asio::make_work_guard(m_ioService)}
    , m_worker{[=] { m_ioService.run(); }}
{
}

Client::~Client()
{
    m_ioService.stop();
    m_worker.join();
}

void Client::connect(ConnectRequest request, Callback<ConnectResponse> callback)
{
    asio::post(m_ioService,
        [ request = std::move(request), callback = std::move(callback) ] {
            try {
                auto connection = std::make_shared<Connection>(request);
                callback(ConnectResponse{LCB_SUCCESS, std::move(connection)});
            }
            catch (lcb_error_t err) {
                callback(ConnectResponse{err, nullptr});
            }
        });
}

void Client::get(ConnectionPtr connection, MultiRequest<GetRequest> request,
    Callback<MultiResponse<GetResponse>> callback)
{
    asio::post(m_ioService, [
        connection = std::move(connection), request = std::move(request),
        callback = std::move(callback)
    ] { callback(connection->get(request)); });
}

void Client::store(ConnectionPtr connection, MultiRequest<StoreRequest> request,
    Callback<MultiResponse<StoreResponse>> callback)
{
    asio::post(m_ioService, [
        connection = std::move(connection), request = std::move(request),
        callback = std::move(callback)
    ] { callback(connection->store(request)); });
}

void Client::remove(ConnectionPtr connection,
    MultiRequest<RemoveRequest> request,
    Callback<MultiResponse<RemoveResponse>> callback)
{
    asio::post(m_ioService, [
        connection = std::move(connection), request = std::move(request),
        callback = std::move(callback)
    ] { callback(connection->remove(request)); });
}

void Client::arithmetic(ConnectionPtr connection,
    MultiRequest<ArithmeticRequest> request,
    Callback<MultiResponse<ArithmeticResponse>> callback)
{
    asio::post(m_ioService, [
        connection = std::move(connection), request = std::move(request),
        callback = std::move(callback)
    ] { callback(connection->arithmetic(request)); });
}

void Client::http(ConnectionPtr connection, HttpRequest request,
    Callback<HttpResponse> callback)
{
    asio::post(m_ioService, [
        connection = std::move(connection), request = std::move(request),
        callback = std::move(callback)
    ] { callback(connection->http(request)); });
}

void Client::durability(ConnectionPtr connection,
    MultiRequest<DurabilityRequest> request, DurabilityRequestOptions options,
    Callback<MultiResponse<DurabilityResponse>> callback)
{
    asio::post(m_ioService, [
        connection = std::move(connection), request = std::move(request),
        options = std::move(options), callback = std::move(callback)
    ] { callback(connection->durability(request, options)); });
}

} // namespace cb
