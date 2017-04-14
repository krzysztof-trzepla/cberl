/**
 * @file durabilityRequest.cc
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#include "durabilityRequest.h"

namespace cb {

DurabilityRequest::DurabilityRequest(Raw raw)
    : m_key{std::get<0>(raw)}
    , m_cas{std::get<1>(raw)}
{
}

const std::string &DurabilityRequest::key() const { return m_key; }

lcb_cas_t DurabilityRequest::cas() const { return m_cas; }

DurabilityRequestOptions::DurabilityRequestOptions(Raw raw)
    : m_persistTo{std::get<0>(raw)}
    , m_replicateTo{std::get<1>(raw)}
{
}

std::int32_t DurabilityRequestOptions::persistTo() const { return m_persistTo; }

std::int32_t DurabilityRequestOptions::replicateTo() const
{
    return m_replicateTo;
}

} // namespace cb
