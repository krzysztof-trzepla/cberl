/**
 * @file client.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef COUCHBASE_CLIENT_H
#define COUCHBASE_CLIENT_H

#include "requests/requests.h"
#include "responses/responses.h"
#include "types.h"

#include <asio/executor_work_guard.hpp>
#include <asio/io_service.hpp>
#include <libcouchbase/couchbase.h>

#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace cb {

class Client {
    template <typename T> using Callback = std::function<void(const T &)>;

public:
    Client();

    ~Client();

    void connect(ConnectRequest, Callback<ConnectResponse> callback);

    void get(ConnectionPtr connection, MultiRequest<GetRequest> request,
        Callback<MultiResponse<GetResponse>> callback);

    void store(ConnectionPtr connection, MultiRequest<StoreRequest> request,
        Callback<MultiResponse<StoreResponse>> callback);

    void remove(ConnectionPtr connection, MultiRequest<RemoveRequest> request,
        Callback<MultiResponse<RemoveResponse>> callback);

    void arithmetic(ConnectionPtr connection,
        MultiRequest<ArithmeticRequest> request,
        Callback<MultiResponse<ArithmeticResponse>> callback);

    void http(ConnectionPtr connection, HttpRequest request,
        Callback<HttpResponse> callback);

    void durability(ConnectionPtr connection,
        MultiRequest<DurabilityRequest> request,
        DurabilityRequestOptions options,
        Callback<MultiResponse<DurabilityResponse>> callback);

private:
    asio::io_service m_ioService;
    asio::executor_work_guard<asio::io_service::executor_type> m_work;
    std::thread m_worker;
};

} // namespace cb

#endif // COUCHBASE_CLIENT_H
