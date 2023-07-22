#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../src/frame/framecontroller.hpp"

// Test fixtures
class TestRunnable: public Runnable {
 public:
    bool succeededStart = false;
    bool succeededMain = false;
    bool succeededStop = false;

    void onStart() override {
        succeededStart = frameController->getCurrentState()
            == FrameController::FrameControllerState::RUNNING;
    }

    void mainLoop() override {
        succeededMain = frameController->getCurrentState()
            == FrameController::FrameControllerState::RUNNING;
        frameController->stop();
        succeededMain = succeededMain && frameController->getCurrentState()
            == FrameController::FrameControllerState::STOPPING;
        return;
    }

    void onStop() override {
        succeededStop = frameController->getCurrentState()
            == FrameController::FrameControllerState::STOPPED;
    }

    void resetFlags() {
        succeededStart = false;
        succeededMain  = false;
        succeededStop  = false;
    }
};

class TestTimerSource: public TimerSource {
 public:
    int waitTime;
    bool didStart = false;
    bool didStop = false;
    bool didWait = false;
    FrameController *controller;
    void setMinimumWaitTime(int waitTimeInMs) override {
        waitTime = waitTimeInMs;
    }
    int startTimer() override {
        didStart = true;
        return 0;
    }
    int stopTimer() override {
        didStop = true;
        return 0;
    }
    int waitForTime() override {
        didWait = true;
        return 0;
    }
};

// Test methods
void checkInitialization(FrameController *frameController) {
    REQUIRE(frameController);
    REQUIRE(frameController->getFrameCap() == 0);
    REQUIRE(frameController->getCurrentState()
        == FrameController::FrameControllerState::STOPPED);
}

void checkSetFrameRate(
        FrameController *frameController,
        TestTimerSource *timerSource
) {
    const int FRAME_CAP = 100;
    frameController->setFrameCap(FRAME_CAP);
    REQUIRE(frameController->getFrameCap() == FRAME_CAP);
    REQUIRE(timerSource->waitTime == 10);
}

void checkMainLoop(
        FrameController *frameController,
        TestRunnable *testRunnable
) {
    testRunnable->resetFlags();
    frameController->start();
    REQUIRE(frameController->getCurrentState()
        == FrameController::FrameControllerState::STOPPED);

    REQUIRE(testRunnable->succeededStart);
    REQUIRE(testRunnable->succeededMain);
    REQUIRE(testRunnable->succeededStop);
}

// Test cases
TEST_CASE("Initializes FrameController and runs main loop",
          "[FrameController]") {
    TestRunnable testRunnable = TestRunnable();
    TestTimerSource testTimerSource = TestTimerSource();

    FrameController frameController =
        FrameController(&testRunnable, &testTimerSource);
    checkInitialization(&frameController);

    testTimerSource.controller = &frameController;
    checkSetFrameRate(&frameController, &testTimerSource);

    checkMainLoop(&frameController, &testRunnable);
}

TEST_CASE("Initializes FrameController without runnable",
          "[FrameController]") {
    FrameController frameController = FrameController();
    frameController.start();
    REQUIRE(frameController.getCurrentState()
        == FrameController::FrameControllerState::STOPPED);
}

TEST_CASE("DefaultTimerSource runs with chrono library",
          "[DefaultTimerSource]") {
    DefaultTimerSource timer = DefaultTimerSource();
    timer.setMinimumWaitTime(100);
    int startTimeStamp = timer.startTimer();
    int stopTimeStamp = timer.stopTimer();
    int waitTime = timer.waitForTime();

    REQUIRE(startTimeStamp > 0);
    REQUIRE(stopTimeStamp > 0);
    REQUIRE(waitTime > 0);
}

TEST_CASE("Runnable interface default implementation") {
    class DefaultRunnable: public Runnable {
     public:
        bool succeeded = false;
        void mainLoop() override {
            succeeded = true;
            frameController->stop();
            return;
        }
    };
    DefaultRunnable defaultRunnable = DefaultRunnable();

    FrameController frameController =
        FrameController(&defaultRunnable);
    frameController.start();

    REQUIRE(frameController.getCurrentState()
        == FrameController::FrameControllerState::STOPPED);
    REQUIRE(defaultRunnable.succeeded);
}
