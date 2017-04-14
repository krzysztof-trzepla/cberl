/**
 * @file getRequest.cc
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#include "getRequest.h"

namespace cb {

GetRequest::GetRequest(Raw raw)
    : m_key{std::get<0>(raw)}
    , m_expiry{std::get<1>(raw)}
    , m_lock{std::get<2>(raw)}
{
}

const std::string &GetRequest::key() const { return m_key; }

lcb_time_t GetRequest::expiry() const { return m_expiry; }

bool GetRequest::lock() const { return m_lock; }

} // namespace cb
