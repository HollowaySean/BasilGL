#ifndef SRC_CONTEXT_BASILCONTEXTLOCK_HPP_
#define SRC_CONTEXT_BASILCONTEXTLOCK_HPP_

#include <string>

#include "BasilContext.hpp"

namespace basil {

class BasilContextLock {
 public:
    explicit BasilContextLock(u_int64_t lockID): lockID(lockID) {
        BasilContext::lock(lockID);
    }

    explicit BasilContextLock(std::string lockName) {
        lockID = std::hash<std::string>{}(lockName);
    }

    ~BasilContextLock() {
        BasilContext::unlock(lockID);
        BasilContext::terminate();
    }

 private:
    u_int64_t lockID;
};

}  // namespace basil

#endif  // SRC_CONTEXT_BASILCONTEXTLOCK_HPP_
