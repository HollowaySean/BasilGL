#pragma once

#include <functional>

#include "IProcess.hpp"

namespace basil {

class LambdaProcess : public IProcess {
 public:
    /** @brief Creates IProcess instance which runs void lambda on loop. */
    explicit LambdaProcess(const std::function<void()>& lambda)
        : lambda(lambda) {}

    /** @brief OnLoop override which invokes lambda */
    void onLoop() override { lambda(); }

 private:
    const std::function<void()>& lambda;
};

}   // namespace basil
