/**
 * @file storeResponse.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_STORE_RESPONSE_H
#define CBERL_STORE_RESPONSE_H

#include "response.h"

namespace cb {

class StoreResponse : public Response {
public:
    StoreResponse(lcb_error_t err, const void *key, std::size_t keySize);

    StoreResponse(const void *key, std::size_t keySize, lcb_cas_t cas);

    nifpp::TERM toTerm(const Env &env) const;

private:
    std::string m_key;
    lcb_cas_t m_cas;
};

} // namespace cb

#endif // CBERL_STORE_RESPONSE_H
