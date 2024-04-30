#ifndef SRC_CONTEXT_BASILCONTEXTLOCK_HPP_
#define SRC_CONTEXT_BASILCONTEXTLOCK_HPP_

#include <mutex>
#include <string>

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

}  // namespace basil

#endif  // SRC_CONTEXT_BASILCONTEXTLOCK_HPP_
