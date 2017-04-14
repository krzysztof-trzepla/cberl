/**
 * @file arithmeticRequest.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_ARITHMETIC_REQUEST_H
#define CBERL_ARITHMETIC_REQUEST_H

#include <libcouchbase/couchbase.h>

#include <string>
#include <tuple>

namespace cb {

class ArithmeticRequest {
public:
    using Raw =
        std::tuple<std::string, std::int64_t, bool, std::uint64_t, lcb_time_t>;

    ArithmeticRequest(Raw raw);

    const std::string &key() const;

    std::int64_t delta() const;

    bool create() const;

    std::uint64_t initial() const;

    lcb_time_t expiry() const;

private:
    std::string m_key;
    std::int64_t m_delta;
    bool m_create;
    std::uint64_t m_initial;
    lcb_time_t m_expiry;
};

} // namespace cb

#endif // CBERL_ARITHMETIC_REQUEST_H
