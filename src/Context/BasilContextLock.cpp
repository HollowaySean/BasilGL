#include "BasilContextLock.hpp"

namespace basil {

std::mutex BasilContextLock::lockMutex = std::mutex();

BasilContextLock::BasilContextLock() {
    lockMutex.lock();
}

BasilContextLock::~BasilContextLock() {
    lockMutex.unlock();
    BasilContext::terminate();
}

};  // namespace basil
