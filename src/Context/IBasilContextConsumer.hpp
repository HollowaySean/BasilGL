#ifndef SRC_CONTEXT_IBASILCONTEXTCONSUMER_HPP_
#define SRC_CONTEXT_IBASILCONTEXTCONSUMER_HPP_

#include "BasilContext.hpp"

namespace basil {

/** @brief Requires derived classes to initialize context. */
class IBasilContextConsumer {
 protected:
    IBasilContextConsumer() {
        BasilContext::initialize();
    }
};

}  // namespace basil

#endif  // SRC_CONTEXT_IBASILCONTEXTCONSUMER_HPP_
