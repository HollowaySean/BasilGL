#ifndef TST_PROCESS_PROCESSTESTUTILS_HPP_
#define TST_PROCESS_PROCESSTESTUTILS_HPP_

#include <Basil/Process.hpp>

using basil::IProcess;

class TestProcess : public IProcess {
 public:
    bool didStart, didLoop, didStop;

    void onStart() override {
        IProcess::onStart();
        didStart = true;
    }

    void onLoop() override {
        didLoop = true;
    }

    void onStop() override {
        IProcess::onStop();
        didStop = true;
    }
};

#endif  // TST_PROCESS_PROCESSTESTUTILS_HPP_
