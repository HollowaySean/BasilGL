#ifndef SRC_CONTEXT_BASILCONTEXTLOCK_HPP_
#define SRC_CONTEXT_BASILCONTEXTLOCK_HPP_

#include <string>

#include "BasilContext.hpp"

namespace basil {

/** @brief Forces single threaded access to BasilContext. */
class BasilContextLock {
 public:
    /** @param lockID   ID of process to hold lock. */
    explicit BasilContextLock(u_int64_t lockID): lockID(lockID) {
        BasilContext::lock(lockID);
    }

    /** @param lockName String to hash into lockID. */
    explicit BasilContextLock(std::string lockName) {
        lockID = std::hash<std::string>{}(lockName);
    }

    /** @brief Unlock and terminate context. */
    ~BasilContextLock() {
        BasilContext::unlock(lockID);
        BasilContext::terminate();
    }

 private:
    u_int64_t lockID;
};

}  // namespace basil

#endif  // SRC_CONTEXT_BASILCONTEXTLOCK_HPP_
