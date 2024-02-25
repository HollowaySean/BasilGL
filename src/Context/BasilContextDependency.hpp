#ifndef SRC_CONTEXT_BASILCONTEXTDEPENDENCY_HPP_
#define SRC_CONTEXT_BASILCONTEXTDEPENDENCY_HPP_

#include "BasilContext.hpp"

namespace basil {

/** @brief Requires derived classes to destroy context. */
class BasilContextDependency : private BasilContextConsumer {
 protected:
    ~BasilContextDependency() {
        BasilContext::terminate();
    }
};

}  // namespace basil

#endif  // SRC_CONTEXT_BASILCONTEXTDEPENDENCY_HPP_
