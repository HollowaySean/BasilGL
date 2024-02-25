#ifndef SRC_CONTEXT_BASILCONTEXTCONSUMER_HPP_
#define SRC_CONTEXT_BASILCONTEXTCONSUMER_HPP_

#include "BasilContext.hpp"

namespace basil {

/** @brief Requires derived classes to initialize context. */
class BasilContextConsumer {
 protected:
    BasilContextConsumer() {
        BasilContext::initialize();
    }
};

}  // namespace basil

#endif  // SRC_CONTEXT_BASILCONTEXTCONSUMER_HPP_
