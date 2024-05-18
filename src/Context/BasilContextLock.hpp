#pragma once

#include <mutex>

#include "BasilContext.hpp"

namespace basil {

/** @brief Forces single threaded access to BasilContext. */
class BasilContextLock {
 public:
    /** @brief Locks BasilContext global object. */
    BasilContextLock() {
        lockMutex.lock();
    }

    /** @brief Unlock and terminate context. */
    ~BasilContextLock() {
        lockMutex.unlock();
        BasilContext::terminate();
    }

 private:
    static inline std::mutex lockMutex = std::mutex();
};

}   // namespace basil
