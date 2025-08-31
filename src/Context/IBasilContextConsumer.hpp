#pragma once

#include "BasilContext.hpp"

namespace basil {

/** @brief Requires derived classes to initialize context. */
class IBasilContextConsumer {
 protected:
    IBasilContextConsumer() : basilContext(BasilContext::get()) {
        basilContext.initialize();
    }

    BasilContext& basilContext;
};

}   // namespace basil
