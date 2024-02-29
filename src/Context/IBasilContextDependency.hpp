#ifndef SRC_CONTEXT_IBASILCONTEXTDEPENDENCY_HPP_
#define SRC_CONTEXT_IBASILCONTEXTDEPENDENCY_HPP_

#include "BasilContext.hpp"

namespace basil {

/** @brief Requires derived classes to destroy context. */
class IBasilContextDependency : private IBasilContextConsumer {
 protected:
    ~IBasilContextDependency() {
        BasilContext::terminate();
    }
};

}  // namespace basil

#endif  // SRC_CONTEXT_IBASILCONTEXTDEPENDENCY_HPP_
