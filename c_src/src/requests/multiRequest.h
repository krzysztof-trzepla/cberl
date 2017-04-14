/**
 * @file multiRequest.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_MULTI_REQUEST_H
#define CBERL_MULTI_REQUEST_H

#include <vector>

namespace cb {

template <class RequestT> class MultiRequest {
public:
    MultiRequest(std::vector<typename RequestT::Raw> rawRequests)
    {
        for (auto &rawRequest : rawRequests) {
            m_requests.emplace_back(std::move(rawRequest));
        }
    }

    const std::vector<RequestT> &requests() const { return m_requests; }

private:
    std::vector<RequestT> m_requests;
};

} // namespace cb

#endif // CBERL_MULTI_REQUEST_H
