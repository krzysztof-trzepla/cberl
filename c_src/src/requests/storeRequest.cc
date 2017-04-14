/**
 * @file storeRequest.cc
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#include "storeRequest.h"

namespace cb {

StoreRequest::StoreRequest(Raw raw)
    : m_operation{static_cast<lcb_storage_t>(std::get<0>(raw))}
    , m_key{std::get<1>(raw)}
    , m_value{std::get<2>(raw)}
    , m_flags{std::get<3>(raw)}
    , m_cas{std::get<4>(raw)}
    , m_expiry{std::get<5>(raw)}
{
}

lcb_storage_t StoreRequest::operation() const { return m_operation; }

const std::string &StoreRequest::key() const { return m_key; }

lcb_cas_t StoreRequest::cas() const { return m_cas; }

lcb_uint32_t StoreRequest::flags() const { return m_flags; }

const std::string &StoreRequest::value() const { return m_value; }

lcb_time_t StoreRequest::expiry() const { return m_expiry; }

} // namespace cb
