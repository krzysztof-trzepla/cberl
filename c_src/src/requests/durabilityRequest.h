/**
 * @file durabilityRequest.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_DURABILITY_REQUEST_H
#define CBERL_DURABILITY_REQUEST_H

#include <libcouchbase/couchbase.h>

#include <string>
#include <tuple>

namespace cb {

class DurabilityRequest {
public:
    using Raw = std::tuple<std::string, lcb_cas_t>;

    DurabilityRequest(Raw raw);

    const std::string &key() const;

    lcb_cas_t cas() const;

private:
    std::string m_key;
    lcb_cas_t m_cas;
};

class DurabilityRequestOptions {
public:
    using Raw = std::tuple<std::int32_t, std::int32_t>;

    DurabilityRequestOptions(Raw raw);

    std::int32_t persistTo() const;

    std::int32_t replicateTo() const;

private:
    std::int32_t m_persistTo;
    std::int32_t m_replicateTo;
};

} // namespace cb

#endif // CBERL_DURABILITY_REQUEST_H
