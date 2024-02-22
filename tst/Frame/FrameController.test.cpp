#include <set>

#include <catch.hpp>

#include <Basil/Frame.hpp>

using basil::FrameController;
using basil::IFrameProcess;
using basil::ITimerSource;
using basil::TimerRecord;
using basil::FrameMetrics;
using basil::FrameMetricsRecord;

using FCState   = FrameController::FCState;
using FPState   = IFrameProcess::State;
using Manager   = FrameController::ProcessManager;
using Iterator  = FrameController::ProcessIterator;
using Instance  = FrameController::ProcessInstance;
using Priv      = FrameController::Privilege;
using Ordinal   = FrameController::Ordinal;

class TestProcess : public IFrameProcess {
 public:
    bool hasRunStart = false;
    bool hasRunLoop = false;
    bool hasRunStop = false;

    void onStart() override {
        hasRunStart = true;
    }
    void onLoop() override {
        hasRunLoop = true;
    }
    void onStop() override {
        hasRunStop = true;
    }
    void clear() {
        hasRunStart = false;
        hasRunLoop = false;
        hasRunStop = false;
        currentState = FPState::READY;
    }
};

class NoOpProcess : public IFrameProcess {
 public:
    bool hasRunLoop = false;
    void onLoop() override {
        hasRunLoop = true;
    }
    void clear() {
        hasRunLoop = false;
    }
};

class TestTimer : public ITimerSource {
 public:
    std::unique_ptr<ITimerSource> clone() const override {
        return std::make_unique<TestTimer>(*this);
    }
    void frameStart() override {
        currentRecord.frameStart = 0.; }
    void frameDone() override {
        currentRecord.frameDone = 1.; }
    void waitForFrameTime() override {
        currentRecord.frameEnd = 2.; }
    void processStart(int pid) override {}
    void processDone(int pid) override {}
    double readTimer() const override { return 0.; }
};

TEST_CASE("Frame_FrameController_FrameController") {
    FrameController controller = FrameController();

    SECTION("Initializes with no frame cap") {
        REQUIRE(controller.getFrameCap() == 0);
    }

    SECTION("Initializes with READY state") {
        REQUIRE(controller.getCurrentState() ==
            FCState::READY);
    }

    SECTION("Passes timer source to process manager") {
        REQUIRE(controller.timerSource ==
            controller.manager.timerSource);
    }
}

TEST_CASE("Frame_FrameController_addProcess") {
    FrameController controller = FrameController();
    TestProcess process = TestProcess();

    controller.addProcess(&process,
        Priv::LOW, "firstProcess");
    Manager *manager = &controller.manager;

    SECTION("Process manager gets process") {
        auto processInstance = manager->processes.back();
        REQUIRE(processInstance->frameProcess == &process);
        REQUIRE(processInstance->privilegeLevel == Priv::LOW);
        REQUIRE(processInstance->processName == "firstProcess");
    }

    SECTION("ProcessID increments per process") {
        auto processInstance = manager->processes.back();
        int processID = processInstance->processID;
        REQUIRE(processID == 0);

        controller.addProcess(&process);

        auto newInstance = manager->processes.back();
        processID = newInstance->processID;
        REQUIRE(processID == 1);
    }

    SECTION("Sets defaults if not provided") {
        controller.addProcess(&process, "newName");

        auto processInstance = manager->processes.back();
        REQUIRE(processInstance->privilegeLevel == Priv::NONE);
        REQUIRE(processInstance->processName == "newName");

        controller.addProcess(&process, Priv::HIGH);

        processInstance = manager->processes.back();
        REQUIRE(processInstance->privilegeLevel == Priv::HIGH);
        REQUIRE(processInstance->processName == "anonymous");
    }

    SECTION("Early and late processes run in correct order") {
        controller.addProcess(&process, Ordinal::EARLY);
        controller.addProcess(&process, Priv::HIGH, Ordinal::LATE);
        controller.addProcess(&process, Ordinal::MAIN);

        auto processUnderTest = manager->processes.begin();
        int processID = (*(processUnderTest))->processID;
        REQUIRE(processID == 1);
        processUnderTest = manager->processes.next();
        processID = (*(processUnderTest))->processID;
        REQUIRE(processID == 0);
        processUnderTest = manager->processes.next();
        processID = (*(processUnderTest))->processID;
        REQUIRE(processID == 3);
        processUnderTest = manager->processes.next();
        processID = (*(processUnderTest))->processID;
        REQUIRE(processID == 2);
    }
}

TEST_CASE("Frame_FrameController_setFrameCap") {
    SECTION("Sets ITimerSource wait time") {
        FrameController controller = FrameController();

        const int FRAME_CAP = 10;
        controller.setFrameCap(FRAME_CAP);

        const double expected = 0.1;
        const double actual = controller.timerSource->waitTimeInSeconds;

        REQUIRE(expected == actual);
        REQUIRE(FRAME_CAP == controller.getFrameCap());
    }
}

TEST_CASE("Frame_FrameController_run") {
    TestTimer timer = TestTimer();
    FrameController controller = FrameController(&timer);
    Manager *manager = &controller.manager;
    FrameMetrics *metrics = &controller.metrics;

    TestProcess firstProcess = TestProcess();
    TestProcess secondProcess = TestProcess();
    NoOpProcess thirdProcess = NoOpProcess();

    controller.addProcess(&firstProcess);
    controller.addProcess(&secondProcess);
    controller.addProcess(&thirdProcess, Priv::HIGH);

    thirdProcess.currentState = FPState::REQUEST_STOP;

    SECTION("Requests that manager runs processes") {
        controller.run();

        REQUIRE(firstProcess.hasRunLoop);
        REQUIRE(secondProcess.hasRunLoop);
        REQUIRE(thirdProcess.hasRunLoop);
    }

    SECTION("Records frame metrics") {
        controller.run();

        FrameMetricsRecord record = metrics->getCurrentMetrics();

        REQUIRE(record.frameTime == 2.);
        REQUIRE(record.workTime == 1.);
    }
}

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

TEST_CASE("Frame_FrameController_shouldRunLoop") {
    FrameController controller = FrameController();
    TestProcess process = TestProcess();

    SECTION("Returns false if no processes are present") {
        controller.currentState = FCState::RUNNING;
        REQUIRE_FALSE(controller.shouldRunLoop());
    }

    SECTION("Returns false if state is elevated") {
        controller.currentState = FCState::KILLED;
        controller.addProcess(&process);
        REQUIRE_FALSE(controller.shouldRunLoop());
    }

    SECTION("Returns true otherwise") {
        controller.currentState = FCState::READY;
        controller.addProcess(&process);
        REQUIRE(controller.shouldRunLoop());
    }
}

TEST_CASE("Frame_ProcessIterator_back") {
    FrameController controller = FrameController();
    Manager *manager = &controller.manager;
    Iterator *iterator = &(manager->processes);

    TestProcess earlyProcess = TestProcess();
    TestProcess mainProcess = TestProcess();
    TestProcess lateProcess = TestProcess();

    SECTION("Returns late.back if it exists") {
        controller.addProcess(&earlyProcess, Ordinal::EARLY);
        controller.addProcess(&mainProcess, Ordinal::MAIN);
        controller.addProcess(&lateProcess, Ordinal::LATE);

        auto back = iterator->back();
        REQUIRE(back->frameProcess == &lateProcess);
    }

    SECTION("Returns main.back if late does not exist") {
        controller.addProcess(&earlyProcess, Ordinal::EARLY);
        controller.addProcess(&mainProcess, Ordinal::MAIN);

        auto back = iterator->back();
        REQUIRE(back->frameProcess == &mainProcess);
    }

    SECTION("Returns early.back if main and late do not exist") {
        controller.addProcess(&earlyProcess, Ordinal::EARLY);

        auto back = iterator->back();
        REQUIRE(back->frameProcess == &earlyProcess);
    }
}

TEST_CASE("Frame_ProcessManager_addProcess") {
    FrameController controller = FrameController();
    TestProcess process = TestProcess();
    Manager *manager = &controller.manager;

    SECTION("Does not add process if null") {
        manager->addProcess(nullptr);
        int processCount = manager->processes.size();

        REQUIRE(processCount == 0);
    }

    SECTION("Adds process to list") {
        controller.addProcess(&process);
        auto newestProcess = manager->processes.back();
        int processCount = manager->processes.size();

        REQUIRE(&process == newestProcess->frameProcess);
        REQUIRE(processCount == 1);
    }
}

TEST_CASE("Frame_ProcessManager_removeProcess") {
    FrameController controller = FrameController();
    Manager *manager = &controller.manager;

    TestProcess earlyProcess = TestProcess();
    Instance *earlyInstance = new Instance {
        &earlyProcess, Priv::NONE, 0, "" };
    TestProcess mainProcessOne = TestProcess();
    Instance *mainInstanceOne  = new Instance {
        &mainProcessOne, Priv::NONE, 1, "" };
    TestProcess mainProcessTwo = TestProcess();
    Instance *mainInstanceTwo  = new Instance {
        &mainProcessTwo, Priv::NONE, 2, "" };
    TestProcess lateProcess = TestProcess();
    Instance *lateInstance     = new Instance {
        &lateProcess, Priv::NONE, 3, "" };

    manager->addProcess(earlyInstance, Ordinal::EARLY);
    manager->addProcess(mainInstanceOne);
    manager->addProcess(mainInstanceTwo);
    manager->addProcess(lateInstance, Ordinal::LATE);

    SECTION("NoOp if process is null") {
        manager->removeProcess(nullptr);
        REQUIRE(manager->processes.size() == 4);
    }

    SECTION("Correctly removes process") {
        manager->removeProcess(mainInstanceTwo);
        REQUIRE(manager->processes.size() == 3);
        manager->processes.begin();
        manager->removeProcess(earlyInstance);
        REQUIRE(manager->processes.size() == 2);
        manager->removeProcess(earlyInstance);
        REQUIRE(manager->processes.size() == 2);
        manager->removeProcess(lateInstance);
        REQUIRE(manager->processes.size() == 1);
        manager->processes.begin();
        manager->removeProcess(mainInstanceOne);
        REQUIRE(manager->processes.size() == 0);
    }
}


TEST_CASE("Frame_ProcessManager_hasProcesses") {
    FrameController controller = FrameController();
    TestProcess process = TestProcess();
    Manager *manager = &controller.manager;

    SECTION("Returns false if no processes have been added") {
        REQUIRE_FALSE(manager->hasProcesses());
    }

    SECTION("Returns true if processes have been added") {
        controller.addProcess(&process);
        REQUIRE(manager->hasProcesses());
    }
}

TEST_CASE("Frame_ProcessManager_runStart") {
    FrameController controller = FrameController();
    Manager *manager = &controller.manager;

    TestProcess firstProcess = TestProcess();
    NoOpProcess secondProcess = NoOpProcess();
    controller.addProcess(&firstProcess);
    controller.addProcess(&secondProcess);

    SECTION("Runs start method for each process") {
        manager->runStart();
        REQUIRE(firstProcess.hasRunStart);

        REQUIRE_FALSE(firstProcess.hasRunLoop);
        REQUIRE_FALSE(secondProcess.hasRunLoop);

        REQUIRE_FALSE(firstProcess.hasRunStop);
    }
}

TEST_CASE("Frame_ProcessManager_runLoop") {
    FrameController controller = FrameController();
    Manager *manager = &controller.manager;

    TestProcess firstProcess = TestProcess();
    NoOpProcess secondProcess = NoOpProcess();
    controller.addProcess(&firstProcess);
    controller.addProcess(&secondProcess);

    SECTION("Runs start method for each process") {
        manager->runLoop();
        REQUIRE_FALSE(firstProcess.hasRunStart);

        REQUIRE(firstProcess.hasRunLoop);
        REQUIRE(secondProcess.hasRunLoop);

        REQUIRE_FALSE(firstProcess.hasRunStop);
    }
}

TEST_CASE("Frame_ProcessManager_runStop") {
    FrameController controller = FrameController();
    Manager *manager = &controller.manager;

    TestProcess firstProcess = TestProcess();
    NoOpProcess secondProcess = NoOpProcess();
    controller.addProcess(&firstProcess);
    controller.addProcess(&secondProcess);

    SECTION("Runs start method for each process") {
        manager->runStop();
        REQUIRE_FALSE(firstProcess.hasRunStart);

        REQUIRE_FALSE(firstProcess.hasRunLoop);
        REQUIRE_FALSE(secondProcess.hasRunLoop);

        REQUIRE(firstProcess.hasRunStop);
    }
}

TEST_CASE("Frame_ProcessManager_runMethod") {
    FrameController controller = FrameController();
    Manager *manager = &controller.manager;
    std::shared_ptr<ITimerSource> timer = controller.timerSource;

    TestProcess firstProcess = TestProcess();
    TestProcess secondProcess = TestProcess();
    NoOpProcess thirdProcess = NoOpProcess();

    bool didRunMethod = false;
    std::function<void(IFrameProcess*)> methodUnderTest =
        [&](IFrameProcess *process) {
            didRunMethod = true;
        };

    SECTION("Runs input lambda on all processes") {
        controller.addProcess(&firstProcess);
        controller.addProcess(&secondProcess);

        manager->runMethod(methodUnderTest);
        REQUIRE(didRunMethod);
    }

    SECTION("Captures process timers") {
        controller.addProcess(&firstProcess);
        controller.addProcess(&secondProcess);

        manager->runMethod(methodUnderTest);
        TimerRecord record = timer->getRecord();

        REQUIRE(record.frameStart > 0);
        REQUIRE(record.frameDone > 0);
        REQUIRE(record.frameEnd > 0);
        REQUIRE(record.processStart.size() > 0);
        REQUIRE(record.processDone.size() > 0);
    }

    SECTION("Kills process when requested by high privilege") {
        controller.addProcess(&firstProcess, Priv::HIGH);
        controller.addProcess(&secondProcess, Priv::LOW);
        controller.addProcess(&thirdProcess, Priv::NONE);

        firstProcess.currentState = FPState::REQUEST_KILL;

        manager->runLoop();
        REQUIRE(manager->currentState == FCState::KILLED);
        REQUIRE_FALSE(firstProcess.hasRunLoop);
        REQUIRE_FALSE(secondProcess.hasRunLoop);
        REQUIRE_FALSE(thirdProcess.hasRunLoop);

        firstProcess.clear();
        secondProcess.clear();
        thirdProcess.clear();
        secondProcess.currentState = FPState::REQUEST_KILL;

        manager->runLoop();
        REQUIRE(manager->currentState == FCState::STOPPING);
        REQUIRE(firstProcess.hasRunLoop);
        REQUIRE(secondProcess.hasRunLoop);
        REQUIRE(thirdProcess.hasRunLoop);

        firstProcess.clear();
        secondProcess.clear();
        thirdProcess.clear();
        thirdProcess.currentState = FPState::REQUEST_KILL;

        manager->runLoop();
        REQUIRE(manager->currentState == FCState::RUNNING);
        REQUIRE(firstProcess.hasRunLoop);
        REQUIRE(secondProcess.hasRunLoop);
        REQUIRE(thirdProcess.hasRunLoop);
    }

    SECTION("Stops process when requested by low privilege") {
        controller.addProcess(&firstProcess, Priv::HIGH);
        controller.addProcess(&secondProcess, Priv::LOW);
        controller.addProcess(&thirdProcess, Priv::NONE);

        firstProcess.currentState = FPState::REQUEST_STOP;

        manager->runLoop();
        REQUIRE(manager->currentState == FCState::STOPPING);
        REQUIRE(firstProcess.hasRunLoop);
        REQUIRE(secondProcess.hasRunLoop);
        REQUIRE(thirdProcess.hasRunLoop);

        firstProcess.clear();
        secondProcess.clear();
        thirdProcess.clear();
        secondProcess.currentState = FPState::REQUEST_STOP;

        manager->runLoop();
        REQUIRE(manager->currentState == FCState::STOPPING);
        REQUIRE(firstProcess.hasRunLoop);
        REQUIRE(secondProcess.hasRunLoop);
        REQUIRE(thirdProcess.hasRunLoop);

        firstProcess.clear();
        secondProcess.clear();
        thirdProcess.clear();
        thirdProcess.currentState = FPState::REQUEST_STOP;

        manager->runLoop();
        REQUIRE(manager->currentState == FCState::RUNNING);
        REQUIRE(firstProcess.hasRunLoop);
        REQUIRE(secondProcess.hasRunLoop);
        REQUIRE(thirdProcess.hasRunLoop);
    }

    SECTION("Skips process when requested by any privilege") {
        controller.addProcess(&firstProcess, Priv::HIGH);
        controller.addProcess(&secondProcess, Priv::LOW);
        controller.addProcess(&thirdProcess, Priv::NONE);

        firstProcess.currentState = FPState::SKIP_PROCESS;

        manager->runLoop();
        REQUIRE(manager->currentState == FCState::RUNNING);
        REQUIRE_FALSE(firstProcess.hasRunLoop);
        REQUIRE(secondProcess.hasRunLoop);
        REQUIRE(thirdProcess.hasRunLoop);

        firstProcess.clear();
        secondProcess.clear();
        thirdProcess.clear();
        secondProcess.currentState = FPState::SKIP_PROCESS;

        manager->runLoop();
        REQUIRE(manager->currentState == FCState::RUNNING);
        REQUIRE(firstProcess.hasRunLoop);
        REQUIRE_FALSE(secondProcess.hasRunLoop);
        REQUIRE(thirdProcess.hasRunLoop);

        firstProcess.clear();
        secondProcess.clear();
        thirdProcess.clear();
        thirdProcess.currentState = FPState::SKIP_PROCESS;

        manager->runLoop();
        REQUIRE(manager->currentState == FCState::RUNNING);
        REQUIRE(firstProcess.hasRunLoop);
        REQUIRE(secondProcess.hasRunLoop);
        REQUIRE_FALSE(thirdProcess.hasRunLoop);
    }

    SECTION("Removes process when requested") {
        controller.addProcess(&firstProcess, Priv::HIGH);
        controller.addProcess(&secondProcess, Priv::LOW);
        controller.addProcess(&thirdProcess, Priv::NONE);

        firstProcess.currentState = FPState::REMOVE_PROCESS;

        manager->runLoop();
        REQUIRE(manager->currentState == FCState::RUNNING);
        REQUIRE_FALSE(firstProcess.hasRunLoop);
        REQUIRE(secondProcess.hasRunLoop);
        REQUIRE(thirdProcess.hasRunLoop);

        int numberOfProcesses = manager->processes.size();
        REQUIRE(numberOfProcesses == 2);
    }
}
