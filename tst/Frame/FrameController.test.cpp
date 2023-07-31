#include <catch.hpp>

#include "FrameController.hpp"

using FCState = FrameController::FCState;

class TestTimer : public ITimerSource {
 public:
    bool frameStarted = false;
    bool frameDoned = false;
    bool frameWaited = false;
    std::list<int> processStarted;
    std::list<int> processDoned;
    bool timerRead = false;

    std::unique_ptr<ITimerSource> clone()
        const { return std::make_unique<TestTimer>(*this); }
    void frameStart() { frameStarted = true; }
    void frameDone() { frameDoned = true; }
    void waitForFrameTime() { frameWaited = true; }
    void processStart(int pid) {
        processStarted.push_back(pid);
    }
    void processDone(int pid) {
        processDoned.push_back(pid);
    }
    double readTimer() const {
        return 0.;
    }

    void clear() {
        frameStarted = false;
        frameDoned = false;
        frameWaited = false;
        processStarted.clear();
        processDoned.clear();
        timerRead = false;
    }
};

TEST_CASE("Frame_FrameController_FrameController") {
    SECTION("Initializes with no frame cap") {
        FrameController controller = FrameController();

        REQUIRE(controller.getFrameCap() == 0);
    }
}

TEST_CASE("Frame_FrameController_setFrameCap") {
    SECTION("Sets ITimerSource wait time") {
        TestTimer timer = TestTimer();
        FrameController controller = FrameController(&timer);

        const int FRAME_CAP = 10;
        controller.setFrameCap(FRAME_CAP);

        const double expected = 0.1;
        const double actual = controller.timerSource->waitTimeInSeconds;

        REQUIRE(expected == actual);
        REQUIRE(FRAME_CAP == controller.getFrameCap());
    }
}

/**
TEST_CASE("Frame_FrameController_start") {

}
*/

TEST_CASE("Frame_FrameController_stop") {
    SECTION("Sets controller state to STOPPING") {
        FrameController controller = FrameController();
        controller.stop();
        REQUIRE(controller.getCurrentState() == FCState::STOPPING);
    }
}

TEST_CASE("Frame_FrameController_kill") {
    SECTION("Sets controller state to KILLED") {
        FrameController controller = FrameController();
        controller.kill();
        REQUIRE(controller.getCurrentState() == FCState::KILLED);
    }
}
/*
TEST_CASE("Frame_FrameController_runProcesses") {

}

TEST_CASE("Frame_ProcessManager_addProcess") {

}

TEST_CASE("Frame_ProcessManager_runStart") {

}

TEST_CASE("Frame_ProcessManager_runLoop") {

}

TEST_CASE("Frame_ProcessManager_runStop") {

}

TEST_CASE("Frame_ProcessManager_runMethod") {

} */
