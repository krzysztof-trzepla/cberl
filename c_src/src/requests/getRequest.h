/**
 * @file getRequest.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_GET_REQUEST_H
#define CBERL_GET_REQUEST_H

#include <libcouchbase/couchbase.h>

#include <string>
#include <tuple>

namespace cb {

class GetRequest {
public:
    using Raw = std::tuple<std::string, lcb_time_t, bool>;

    GetRequest(Raw raw);

    const std::string &key() const;

    lcb_time_t expiry() const;

    bool lock() const;

private:
    std::string m_key;
    lcb_time_t m_expiry;
    bool m_lock;
};

} // namespace cb

#endif // CBERL_GET_REQUEST_H
