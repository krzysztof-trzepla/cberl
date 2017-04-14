/**
 * @file arithmeticResponse.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_ARITHMETIC_RESPONSE_H
#define CBERL_ARITHMETIC_RESPONSE_H

#include "response.h"

namespace cb {

class ArithmeticResponse : public Response {
public:
    ArithmeticResponse(lcb_error_t err, const void *key, std::size_t keySize);

    ArithmeticResponse(const void *key, std::size_t keySize, lcb_cas_t cas,
        std::uint64_t value);

    nifpp::TERM toTerm(const Env &env) const;

private:
    std::string m_key;
    lcb_cas_t m_cas;
    std::uint64_t m_value;
};

} // namespace cb

#endif // CBERL_ARITHMETIC_RESPONSE_H
