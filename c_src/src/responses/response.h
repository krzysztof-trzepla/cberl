/**
 * @file response.h
 * @author Krzysztof Trzepla
 * @copyright (C) 2017: Krzysztof Trzepla
 * This software is released under the MIT license cited in 'LICENSE.md'
 */

#ifndef CBERL_RESPONSE_H
#define CBERL_RESPONSE_H

#include "nifpp.h"

#include <libcouchbase/couchbase.h>

#include <memory>
#include <string>

class Env {
public:
    Env()
        : m_env{enif_alloc_env(), enif_free_env}
    {
    }

    operator ErlNifEnv *() const { return m_env.get(); }

    ErlNifEnv *get() const { return m_env.get(); }

private:
    std::shared_ptr<ErlNifEnv> m_env;
};

namespace cb {

class Response {
public:
    Response(lcb_error_t err = LCB_SUCCESS);

    nifpp::TERM toTerm(const Env &env) const;

protected:
    lcb_error_t m_err;

private:
    std::string errorMessage() const;
};

} // namespace cb

#endif // CBERL_RESPONSE_H
