/**
 * @file removeRequest.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_REMOVE_REQUEST_H
#define CBERL_REMOVE_REQUEST_H

#include <libcouchbase/couchbase.h>

#include <string>
#include <tuple>

namespace cb {

class RemoveRequest {
public:
    using Raw = std::tuple<std::string, lcb_cas_t>;

    RemoveRequest(Raw raw);

    const std::string &key() const;

    lcb_cas_t cas() const;

private:
    std::string m_key;
    lcb_cas_t m_cas;
};

} // namespace cb

#endif // CBERL_REMOVE_REQUEST_H
