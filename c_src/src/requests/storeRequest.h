/**
 * @file storeRequest.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_STORE_REQUEST_H
#define CBERL_STORE_REQUEST_H

#include <libcouchbase/couchbase.h>

#include <string>
#include <tuple>

namespace cb {

class StoreRequest {
public:
    using Raw = std::tuple<int, std::string, std::string, lcb_uint32_t,
        lcb_cas_t, lcb_time_t>;

    StoreRequest(Raw raw);

    lcb_storage_t operation() const;

    const std::string &key() const;

    const std::string &value() const;

    lcb_uint32_t flags() const;

    lcb_cas_t cas() const;

    lcb_time_t expiry() const;

private:
    lcb_storage_t m_operation;
    std::string m_key;
    std::string m_value;
    lcb_uint32_t m_flags;
    lcb_cas_t m_cas;
    lcb_time_t m_expiry;
};

} // namespace cb

#endif // CBERL_STORE_REQUEST_H
