/**
 * @file arithmeticRequest.cc
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#include "arithmeticRequest.h"

namespace cb {

ArithmeticRequest::ArithmeticRequest(Raw raw)
    : m_key{std::get<0>(raw)}
    , m_delta{std::get<1>(raw)}
    , m_create{std::get<2>(raw)}
    , m_initial{std::get<3>(raw)}
    , m_expiry{std::get<4>(raw)}
{
}

const std::string &ArithmeticRequest::key() const { return m_key; }

std::int64_t ArithmeticRequest::delta() const { return m_delta; }

bool ArithmeticRequest::create() const { return m_create; }

std::uint64_t ArithmeticRequest::initial() const { return m_initial; }

lcb_time_t ArithmeticRequest::expiry() const { return m_expiry; }

} // namespace cb
