#pragma once

#include "BasilContext.hpp"

namespace basil {

/** @brief Requires derived classes to destroy context. */
class IBasilContextDependency : private IBasilContextConsumer {
 protected:
    ~IBasilContextDependency() {
        BasilContext::terminate();
    }
};

}   // namespace basil
