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
    BasilContextLock();

    /** @brief Unlock and terminate context. */
    ~BasilContextLock();

 private:
    static std::mutex lockMutex;
};

}  // namespace basil

#endif  // SRC_CONTEXT_BASILCONTEXTLOCK_HPP_
