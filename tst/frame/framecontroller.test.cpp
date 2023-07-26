#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../src/frame/framecontroller.hpp"

/**
 * @brief Tests all classes in src/frame/ directory.
 */

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
    float waitTime;
    bool didStart = false;
    bool didStop = false;
    bool didWait = false;
    FrameController *controller;
    void setMinimumWaitTime(float waitTimeInMs) override {
        waitTime = waitTimeInMs;
    }
    float startTimer() override {
        didStart = true;
        return 0.0;
    }
    float readTimer() override {
        return 0.0;
    }
    float stopTimer() override {
        didStop = true;
        return 0.0;
    }
    float waitForTime() override {
        didWait = true;
        return 0.0;
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

TEST_CASE("Initializes FrameController with multiple runnables",
          "[FrameController]") {
    class TestRunnable: public Runnable {
     public:
        bool hasRun = false;
        void mainLoop() override {
            hasRun = true;
            frameController->stop();
        }
    };
    class HaltingTestRunnable: public Runnable {
     public:
        bool hasRun = false;
        void mainLoop() override {
            hasRun = true;
            frameController->kill();
        }
    };

    TestRunnable first = TestRunnable();
    TestRunnable second = TestRunnable();
    HaltingTestRunnable halting = HaltingTestRunnable();

    FrameController frameController = FrameController();
    frameController.addRunnable(&second, FrameController::LAST);
    frameController.addRunnable(&first, FrameController::FIRST);
    frameController.start();

    REQUIRE(first.hasRun);
    REQUIRE(second.hasRun);

    first.hasRun = false;
    second.hasRun = false;
    frameController.removeRunnable(&first);
    frameController.start();

    REQUIRE_FALSE(first.hasRun);
    REQUIRE(second.hasRun);

    second.hasRun = false;
    frameController.addRunnable(&halting, FrameController::FIRST);
    frameController.addRunnable(&first, FrameController::FIRST);
    frameController.start();

    REQUIRE(first.hasRun);
    REQUIRE_FALSE(second.hasRun);
    REQUIRE(halting.hasRun);
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

TEST_CASE("DefaultTimerSource runs with chrono library",
          "[DefaultTimerSource]") {
    DefaultTimerSource timer = DefaultTimerSource();
    timer.setMinimumWaitTime(100);
    float startTimeStamp = timer.startTimer();
    float stopTimeStamp = timer.stopTimer();
    float waitTime = timer.waitForTime();

    REQUIRE(startTimeStamp > 0);
    REQUIRE(stopTimeStamp > 0);
    REQUIRE(waitTime > 0);
}

TEST_CASE("FrameMetrics tracks and averages time intervals",
          "[FrameMetrics]") {
    // Returns dummy values when empty
    FrameMetrics metrics = FrameMetrics();
    FrameMetrics::MetricsReport report = metrics.getMetricsReport();

    REQUIRE(report.frameRate == -1.0);
    REQUIRE(report.uncappedFrameRate == -1.0);
    REQUIRE(report.frameTimeMS == -1.0);
    REQUIRE(report.workTimeMS == -1.0);
    REQUIRE(report.runTimesMS.size() == 0);

    // Correct basic behavior
    FrameMetrics::ReportList reportList;
    reportList.push_back(FrameMetrics::TimeReport(2.0, "First runner"));
    reportList.push_back(FrameMetrics::TimeReport(0.0, "Second runner"));

    metrics.setSmoothingWindow(2);
    metrics.updateTimeStamps(0.0, 10.0, 10.0, reportList);
    report = metrics.getMetricsReport();

    REQUIRE(report.frameRate == 50.0);
    REQUIRE(report.uncappedFrameRate == 100.0);
    REQUIRE(report.frameTimeMS == 20.0);
    REQUIRE(report.workTimeMS == 10.0);

    REQUIRE(report.runTimesMS.front().second == "First runner");
    REQUIRE(report.runTimesMS.front().first == 2.0);
    REQUIRE(report.runTimesMS.back().second == "Second runner");
    REQUIRE(report.runTimesMS.back().first == 0.0);

    reportList.clear();
    reportList.push_back(FrameMetrics::TimeReport(4.0, "First runner"));
    reportList.push_back(FrameMetrics::TimeReport(2.0, "Second runner"));

    metrics.updateTimeStamps(0.0, 0.0, 0.0, reportList);
    report = metrics.getMetricsReport();
    REQUIRE(report.frameRate == 100.0);
    REQUIRE(report.uncappedFrameRate == 200.0);
    REQUIRE(report.frameTimeMS == 10.0);
    REQUIRE(report.workTimeMS == 5.0);

    REQUIRE(report.runTimesMS.front().second == "First runner");
    REQUIRE(report.runTimesMS.front().first == 3.0);
    REQUIRE(report.runTimesMS.back().second == "Second runner");
    REQUIRE(report.runTimesMS.back().first == 1.0);

    // Should clear data when smoothing window is reduced
    metrics.setSmoothingWindow(5);
    metrics.setSmoothingWindow(2);
    metrics.updateTimeStamps(0.0, 10.0, 10.0, reportList);

    report = metrics.getMetricsReport();
    REQUIRE(report.frameRate == 50.0);
    REQUIRE(report.uncappedFrameRate == 100.0);
    REQUIRE(report.frameTimeMS == 20.0);
    REQUIRE(report.workTimeMS == 10.0);

    REQUIRE(report.runTimesMS.front().second == "First runner");
    REQUIRE(report.runTimesMS.front().first == 4.0);
    REQUIRE(report.runTimesMS.back().second == "Second runner");
    REQUIRE(report.runTimesMS.back().first == 2.0);

    // Should operate correctly when at window maximum
    metrics.updateTimeStamps(0.0, 0.0, 0.0, reportList);
    metrics.updateTimeStamps(0.0, 5.0, 5.0, reportList);

    report = metrics.getMetricsReport();
    REQUIRE(report.frameRate == 200.0);
    REQUIRE(report.uncappedFrameRate == 400.0);
    REQUIRE(report.frameTimeMS == 5.0);
    REQUIRE(report.workTimeMS == 2.5);

    REQUIRE(report.runTimesMS.front().second == "First runner");
    REQUIRE(report.runTimesMS.front().first == 4.0);
    REQUIRE(report.runTimesMS.back().second == "Second runner");
    REQUIRE(report.runTimesMS.back().first == 2.0);
}
