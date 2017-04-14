/**
 * @file connection.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef COUCHBASE_CONNECTION_H
#define COUCHBASE_CONNECTION_H

#include "requests/requests.h"
#include "responses/responses.h"

#include <libcouchbase/couchbase.h>

#include <string>

namespace cb {

class Connection {
public:
    Connection(const ConnectRequest &bucket);

    ~Connection();

    MultiResponse<GetResponse> get(const MultiRequest<GetRequest> &request);

    MultiResponse<StoreResponse> store(
        const MultiRequest<StoreRequest> &request);

    MultiResponse<RemoveResponse> remove(
        const MultiRequest<RemoveRequest> &request);

    MultiResponse<ArithmeticResponse> arithmetic(
        const MultiRequest<ArithmeticRequest> &request);

    HttpResponse http(const HttpRequest &request);

    MultiResponse<DurabilityResponse> durability(
        const MultiRequest<DurabilityRequest> &request,
        const DurabilityRequestOptions &options);

private:
    lcb_t m_instance;
};

} // namespace cb

#endif // COUCHBASE_CONNECTION_H
