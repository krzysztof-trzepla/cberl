/**
 * @file removeRequest.cc
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#include "removeRequest.h"

namespace cb {

RemoveRequest::RemoveRequest(Raw raw)
    : m_key{std::get<0>(raw)}
    , m_cas{std::get<1>(raw)}
{
}

const std::string &RemoveRequest::key() const { return m_key; }

lcb_cas_t RemoveRequest::cas() const { return m_cas; }

} // namespace cb
