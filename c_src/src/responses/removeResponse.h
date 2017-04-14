/**
 * @file removeResponse.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_REMOVE_RESPONSE_H
#define CBERL_REMOVE_RESPONSE_H

#include "response.h"

namespace cb {

class RemoveResponse : public Response {
public:
    RemoveResponse(lcb_error_t err, const void *key, std::size_t keySize);

    nifpp::TERM toTerm(const Env &env) const;

private:
    std::string m_key;
};

} // namespace cb

#endif // CBERL_REMOVE_RESPONSE_H
