/**
 * @file getResponse.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_GET_RESPONSE_H
#define CBERL_GET_RESPONSE_H

#include "response.h"

namespace cb {

class GetResponse : public Response {
public:
    GetResponse(lcb_error_t err, const void *key, std::size_t keySize);

    GetResponse(const void *key, std::size_t keySize, lcb_cas_t cas,
        lcb_uint32_t flags, const void *value, std::size_t valueSize);

    nifpp::TERM toTerm(const Env &env) const;

private:
    std::string m_key;
    lcb_cas_t m_cas;
    lcb_uint32_t m_flags;
    std::string m_value;
};

} // namespace cb

#endif // CBERL_GET_RESPONSE_H
