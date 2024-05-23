#include <catch.hpp>

#include "Window/SplitPane.hpp"

#include "OpenGL/GLTestUtils.hpp"

using basil::PaneProps;
using basil::IPane;
using basil::SplitPane;
using basil::Logger;
using basil::LogLevel;

class TestPane : public IPane {
 public:
    explicit TestPane(PaneProps paneProps):
        IPane(paneProps) {}
    void const draw() override {
        didDraw = true;
    }
    bool didDraw = false;
};

TEST_CASE("Window_SplitPane_draw") {
    auto firstPane = std::make_shared<TestPane>(testPaneProps);
    auto secondPane = std::make_shared<TestPane>(testPaneProps);

    SECTION("Calls draw on child panes") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPane::Orientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        CHECK(firstPane->didDraw == false);
        CHECK(secondPane->didDraw == false);

        splitPane.draw();

        CHECK(firstPane->didDraw == true);
        CHECK(secondPane->didDraw == true);
    }
}

TEST_CASE("Window_SplitPane_onResize") {
    SECTION("Sets props of pane") {
        SplitPane splitPane = SplitPane(testPaneProps);

        splitPane.onResize(10, 5);

        CHECK(splitPane.paneProps.width   == 10);
        CHECK(splitPane.paneProps.height  == 5);
        CHECK(splitPane.paneProps.xOffset == testPaneProps.xOffset);
        CHECK(splitPane.paneProps.yOffset == testPaneProps.yOffset);
    }

    SECTION("Resizes split") {
        SplitPane splitPane = SplitPane(testPaneProps);
        int setExtent = 2;
        splitPane.setPaneSizeInPixels(setExtent);

        CHECK(splitPane.firstPaneExtent == setExtent);
        CHECK(splitPane.secondPaneExtent == testPaneProps.width - setExtent);

        splitPane.onResize(10, 5);

        CHECK(splitPane.firstPaneExtent == setExtent);
        CHECK(splitPane.secondPaneExtent == 10 - setExtent);
    }
}

TEST_CASE("Window_SplitPane_setFirstPane") {
    auto childPane = std::make_shared<TestPane>(testPaneProps);
    auto secondChild = std::make_shared<TestPane>(testPaneProps);

    SECTION("Sets props of child pane for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPane::Orientation::HORIZONTAL);
        splitPane.setFirstPane(childPane);

        CHECK(childPane->paneProps.height == testPaneProps.height);
        CHECK(childPane->paneProps.width == testPaneProps.width / 2);
        CHECK(childPane->paneProps.xOffset == testPaneProps.xOffset);
        CHECK(childPane->paneProps.yOffset == testPaneProps.yOffset);
    }

    SECTION("Sets props of child pane for vertical orientation") {
        auto splitPane = std::make_shared<SplitPane>(
            testPaneProps, SplitPane::Orientation::VERTICAL);
        splitPane->setFirstPane(childPane);

        CHECK(childPane->paneProps.height == testPaneProps.height / 2);
        CHECK(childPane->paneProps.width == testPaneProps.width);
        CHECK(childPane->paneProps.xOffset == testPaneProps.xOffset);
        CHECK(childPane->paneProps.yOffset == testPaneProps.yOffset);
    }

    SECTION("Logs warning if duplicate pane") {
        Logger& logger = Logger::get();

        auto splitPane = std::make_shared<SplitPane>();
        splitPane->setSecondPane(childPane);
        splitPane->setFirstPane(childPane);

        CHECK(splitPane->firstPane == nullptr);
        CHECK(logger.getLastOutput() ==
            "First pane has same address as second. Pane not set.");
        CHECK(logger.getLastLevel() == LogLevel::WARN);
    }

    SECTION("Subscribes & unsubscribes IDataSubscribers") {
        auto splitPane = SplitPane();

        splitPane.setFirstPane(childPane);
        CHECK(splitPane.subscriptions.contains(childPane));

        splitPane.setFirstPane(secondChild);
        CHECK(splitPane.subscriptions.contains(secondChild));
        CHECK_FALSE(splitPane.subscriptions.contains(childPane));
    }
}

TEST_CASE("Window_SplitPane_setSecondPane") {
    auto childPane = std::make_shared<TestPane>(testPaneProps);
    auto secondChild = std::make_shared<TestPane>(testPaneProps);

    SECTION("Sets props of child pane for horizontal orientation") {
        auto splitPane = std::make_shared<SplitPane>(
            testPaneProps, SplitPane::Orientation::HORIZONTAL);
        splitPane->setSecondPane(childPane);

        CHECK(childPane->paneProps.height == testPaneProps.height);
        CHECK(childPane->paneProps.width == testPaneProps.width / 2);
        CHECK(childPane->paneProps.xOffset ==
            testPaneProps.xOffset + testPaneProps.width / 2);
        CHECK(childPane->paneProps.yOffset == testPaneProps.yOffset);
    }

    SECTION("Sets props of child pane for vertical orientation") {
        auto splitPane = std::make_shared<SplitPane>(
            testPaneProps, SplitPane::Orientation::VERTICAL);
        splitPane->setSecondPane(childPane);

        CHECK(childPane->paneProps.height == testPaneProps.height / 2);
        CHECK(childPane->paneProps.width == testPaneProps.width);
        CHECK(childPane->paneProps.xOffset == testPaneProps.xOffset);
        CHECK(childPane->paneProps.yOffset ==
            testPaneProps.yOffset + testPaneProps.height / 2);
    }

    SECTION("Logs warning if duplicate pane") {
        Logger& logger = Logger::get();

        auto splitPane = std::make_shared<SplitPane>();
        splitPane->setFirstPane(childPane);
        splitPane->setSecondPane(childPane);

        CHECK(splitPane->secondPane == nullptr);
        CHECK(logger.getLastOutput() ==
            "Second pane has same address as first. Pane not set.");
        CHECK(logger.getLastLevel() == LogLevel::WARN);
    }

    SECTION("Subscribes & unsubscribes IDataSubscribers") {
        auto splitPane = SplitPane();

        splitPane.setSecondPane(childPane);
        CHECK(splitPane.subscriptions.contains(childPane));

        splitPane.setSecondPane(secondChild);
        CHECK(splitPane.subscriptions.contains(secondChild));
        CHECK_FALSE(splitPane.subscriptions.contains(childPane));
    }
}

TEST_CASE("Window_SplitPane_setFixedPane") {
    auto splitPane = SplitPane(testPaneProps);
    splitPane.setPaneSizeInPixels(30);

    SECTION("Sets fixed pane and resizes") {
        CHECK(splitPane.getFirstPaneSizeInPixels() == 30);
        splitPane.setFixedPane(SplitPane::FixedPane::SECOND);
        CHECK(splitPane.getSecondPaneSizeInPixels() == 30);
    }
}

TEST_CASE("Window_SplitPane_setPaneSizeInPixels") {
    auto firstPane = std::make_shared<TestPane>(testPaneProps);
    auto secondPane = std::make_shared<TestPane>(testPaneProps);

    SECTION("Updates sizes of child panes for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPane::Orientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        int paneSize = 50;
        splitPane.setPaneSizeInPixels(paneSize);

        CHECK(firstPane->paneProps.width
            == paneSize);
        CHECK(firstPane->paneProps.height
            == testPaneProps.height);
        CHECK(firstPane->paneProps.xOffset
            == testPaneProps.xOffset);
        CHECK(firstPane->paneProps.yOffset
            == testPaneProps.yOffset);

        CHECK(secondPane->paneProps.width
            == testPaneProps.width - paneSize);
        CHECK(secondPane->paneProps.height
            == testPaneProps.height);
        CHECK(secondPane->paneProps.xOffset
            == testPaneProps.xOffset + paneSize);
        CHECK(secondPane->paneProps.yOffset
            == testPaneProps.yOffset);

        CHECK(splitPane.getFirstPaneSizeInPixels()
            == paneSize);
        CHECK(splitPane.getSecondPaneSizeInPixels()
            == testPaneProps.width - paneSize);
    }

    SECTION("Updates sizes of child panes for vertical orientation") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPane::Orientation::VERTICAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        int paneSize = 50;
        splitPane.setPaneSizeInPixels(paneSize);

        CHECK(firstPane->paneProps.width
            == testPaneProps.width);
        CHECK(firstPane->paneProps.height
            == paneSize);
        CHECK(firstPane->paneProps.xOffset
            == testPaneProps.xOffset);
        CHECK(firstPane->paneProps.yOffset
            == testPaneProps.yOffset);

        CHECK(secondPane->paneProps.width
            == testPaneProps.width);
        CHECK(secondPane->paneProps.height
            == testPaneProps.height - paneSize);
        CHECK(secondPane->paneProps.xOffset
            == testPaneProps.xOffset);
        CHECK(secondPane->paneProps.yOffset
            == testPaneProps.yOffset + paneSize);

        CHECK(splitPane.getFirstPaneSizeInPixels()
            == paneSize);
        CHECK(splitPane.getSecondPaneSizeInPixels()
            == testPaneProps.height - paneSize);
    }

    SECTION("Clamps value if extent out of bounds") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPane::Orientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.setPaneSizeInPixels(-5);
        CHECK(firstPane->paneProps.width == 0);

        CHECK(splitPane.getFirstPaneSizeInPixels()
            == 0.);
        CHECK(splitPane.getSecondPaneSizeInPixels()
            == testPaneProps.width);

        splitPane.setPaneSizeInPixels(10000.f);
        CHECK(firstPane->paneProps.width == testPaneProps.width);

        CHECK(splitPane.getFirstPaneSizeInPixels()
            == testPaneProps.width);
        CHECK(splitPane.getSecondPaneSizeInPixels()
            == 0.);
    }
}

TEST_CASE("Window_SplitPane_setPaneSizeAsPercentage") {
    auto firstPane = std::make_shared<TestPane>(testPaneProps);
    auto secondPane = std::make_shared<TestPane>(testPaneProps);

    SECTION("Updates sizes of child panes for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPane::Orientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        float setPercentage = 25.f;
        splitPane.setPaneSizeAsPercentage(setPercentage);

        int paneSize = testPaneProps.width * (setPercentage / 100.);

        CHECK(firstPane->paneProps.width
            == paneSize);
        CHECK(firstPane->paneProps.height
            == testPaneProps.height);
        CHECK(firstPane->paneProps.xOffset
            == testPaneProps.xOffset);
        CHECK(firstPane->paneProps.yOffset
            == testPaneProps.yOffset);

        CHECK(secondPane->paneProps.width
            == testPaneProps.width - paneSize);
        CHECK(secondPane->paneProps.height
            == testPaneProps.height);
        CHECK(secondPane->paneProps.xOffset
            == testPaneProps.xOffset + paneSize);
        CHECK(secondPane->paneProps.yOffset
            == testPaneProps.yOffset);

        CHECK(splitPane.getFirstPaneSizeAsPercentage()
            == setPercentage);
        CHECK(splitPane.getSecondPaneSizeAsPercentage()
            == 100. - setPercentage);
    }

    SECTION("Updates sizes of child panes for vertical orientation") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPane::Orientation::VERTICAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        float setPercentage = 25.f;
        splitPane.setPaneSizeAsPercentage(setPercentage);

        int paneSize = testPaneProps.height * (setPercentage / 100.);

        CHECK(firstPane->paneProps.width
            == testPaneProps.width);
        CHECK(firstPane->paneProps.height
            == paneSize);
        CHECK(firstPane->paneProps.xOffset
            == testPaneProps.xOffset);
        CHECK(firstPane->paneProps.yOffset
            == testPaneProps.yOffset);

        CHECK(secondPane->paneProps.width
            == testPaneProps.width);
        CHECK(secondPane->paneProps.height
            == testPaneProps.height - paneSize);
        CHECK(secondPane->paneProps.xOffset
            == testPaneProps.xOffset);
        CHECK(secondPane->paneProps.yOffset
            == testPaneProps.yOffset + paneSize);

        CHECK(splitPane.getFirstPaneSizeAsPercentage()
            == setPercentage);
        CHECK(splitPane.getSecondPaneSizeAsPercentage()
            == 100. - setPercentage);
    }

    SECTION("Clamps value if extent out of bounds") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPane::Orientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.setPaneSizeAsPercentage(-5.f);
        CHECK(firstPane->paneProps.width == 0);

        CHECK(splitPane.getFirstPaneSizeAsPercentage() == 0.);
        CHECK(splitPane.getSecondPaneSizeAsPercentage() == 100.);

        splitPane.setPaneSizeAsPercentage(120.f);
        CHECK(firstPane->paneProps.width == testPaneProps.width);

        CHECK(splitPane.getFirstPaneSizeAsPercentage() == 100.);
        CHECK(splitPane.getSecondPaneSizeAsPercentage() == 0.);
    }
}

TEST_CASE("Window_SplitPane_setPaneAspectRatio") {
    auto firstPane = std::make_shared<TestPane>(testPaneProps);
    auto secondPane = std::make_shared<TestPane>(testPaneProps);

    SECTION("Updates sizes of child panes for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPane::Orientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        float setRatio = 1.25f;
        splitPane.setPaneAspectRatio(setRatio);

        int paneSize = testPaneProps.height * setRatio;

        CHECK(firstPane->paneProps.width
            == paneSize);
        CHECK(firstPane->paneProps.height
            == testPaneProps.height);
        CHECK(firstPane->paneProps.xOffset
            == testPaneProps.xOffset);
        CHECK(firstPane->paneProps.yOffset
            == testPaneProps.yOffset);

        CHECK(secondPane->paneProps.width
            == testPaneProps.width - paneSize);
        CHECK(secondPane->paneProps.height
            == testPaneProps.height);
        CHECK(secondPane->paneProps.xOffset
            == testPaneProps.xOffset + paneSize);
        CHECK(secondPane->paneProps.yOffset
            == testPaneProps.yOffset);

        CHECK(splitPane.getFirstPaneAspectRatio()
            == setRatio);
        CHECK(splitPane.getSecondPaneAspectRatio()
            == static_cast<float>((testPaneProps.width - paneSize))
                / testPaneProps.height);
    }

    SECTION("Updates sizes of child panes for vertical orientation") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPane::Orientation::VERTICAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        float setRatio = 2.5f;
        splitPane.setPaneAspectRatio(setRatio);

        int paneSize = testPaneProps.width / setRatio;

        CHECK(firstPane->paneProps.width
            == testPaneProps.width);
        CHECK(firstPane->paneProps.height
            == paneSize);
        CHECK(firstPane->paneProps.xOffset
            == testPaneProps.xOffset);
        CHECK(firstPane->paneProps.yOffset
            == testPaneProps.yOffset);

        CHECK(secondPane->paneProps.width
            == testPaneProps.width);
        CHECK(secondPane->paneProps.height
            == testPaneProps.height - paneSize);
        CHECK(secondPane->paneProps.xOffset
            == testPaneProps.xOffset);
        CHECK(secondPane->paneProps.yOffset
            == testPaneProps.yOffset + paneSize);

        CHECK(splitPane.getFirstPaneAspectRatio()
            == setRatio);
        CHECK(splitPane.getSecondPaneAspectRatio()
            == static_cast<float>(testPaneProps.width
                / (testPaneProps.height - paneSize)));
    }

    SECTION("Clamps value if extent out of bounds") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPane::Orientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.setPaneAspectRatio(-5.f);
        CHECK(firstPane->paneProps.width == 0);

        CHECK(splitPane.getFirstPaneAspectRatio() == 0);

        splitPane.setPaneAspectRatio(100.f);
        CHECK(firstPane->paneProps.width == testPaneProps.width);

        CHECK(splitPane.getFirstPaneAspectRatio()
            == splitPane.getTotalAspect());
    }
}

TEST_CASE("Window_SplitPane_setGapWidth") {
    auto firstPane = std::make_shared<TestPane>(testPaneProps);
    auto secondPane = std::make_shared<TestPane>(testPaneProps);

    SECTION("Resizes to include gap, for horizontal orientation") {
        SplitPane splitPane = SplitPane(testPaneProps);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        int paneSize = 50;
        splitPane.setPaneSizeInPixels(paneSize);

        int gapWidth = 4;
        splitPane.setGapWidth(gapWidth);

        CHECK(splitPane.getGapWidth() == gapWidth);

        CHECK(firstPane->paneProps.width
            == paneSize);
        CHECK(firstPane->paneProps.height
            == testPaneProps.height);
        CHECK(firstPane->paneProps.xOffset
            == testPaneProps.xOffset);
        CHECK(firstPane->paneProps.yOffset
            == testPaneProps.yOffset);

        CHECK(secondPane->paneProps.width
            == testPaneProps.width - paneSize - gapWidth);
        CHECK(secondPane->paneProps.height
            == testPaneProps.height);
        CHECK(secondPane->paneProps.xOffset
            == testPaneProps.xOffset + paneSize + gapWidth);
        CHECK(secondPane->paneProps.yOffset
            == testPaneProps.yOffset);
    }
}

TEST_CASE("Window_SplitPane_setOrientation") {
    SECTION("Sets pane orientation") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPane::Orientation::HORIZONTAL);

        splitPane.setOrientation(SplitPane::Orientation::VERTICAL);

        CHECK(splitPane.getOrientation() == SplitPane::Orientation::VERTICAL);
    }
}

TEST_CASE("Window_SplitPane_Builder") {
    auto firstPane = std::make_shared<TestPane>(testPaneProps);
    auto secondPane = std::make_shared<TestPane>(testPaneProps);

    SECTION("Builds SplitPane object with set percentage") {
        auto splitPane = SplitPane::Builder()
            .withPaneProps(testPaneProps)
            .withFirstPane(firstPane)
            .withSecondPane(secondPane)
            .withPaneExtentInPercent(30.)
            .withGapWidth(3)
            .withOrientation(SplitPane::Orientation::VERTICAL)
            .build();

        CHECK(splitPane->firstPane == firstPane);
        CHECK(splitPane->secondPane == secondPane);
        CHECK(splitPane->getGapWidth() == 3);
        CHECK(splitPane->getFirstPaneSizeAsPercentage() == 30.);
        CHECK(splitPane->getOrientation()
            == SplitPane::Orientation::VERTICAL);
    }


    SECTION("Builds SplitPane object with set pixels") {
        auto splitPane = SplitPane::Builder()
            .withPaneProps(testPaneProps)
            .withFirstPane(firstPane)
            .withSecondPane(secondPane)
            .withPaneExtentInPixels(15)
            .withOrientation(SplitPane::Orientation::HORIZONTAL)
            .build();

        CHECK(splitPane->firstPane == firstPane);
        CHECK(splitPane->secondPane == secondPane);
        CHECK(splitPane->getGapWidth() == 0);
        CHECK(splitPane->getFirstPaneSizeInPixels() == 15);
        CHECK(splitPane->getOrientation()
            == SplitPane::Orientation::HORIZONTAL);
    }


    SECTION("Builds SplitPane object with set aspect ratio") {
        auto splitPane = SplitPane::Builder()
            .withPaneProps(testPaneProps)
            .withFirstPane(firstPane)
            .withSecondPane(secondPane)
            .withPaneAspectRatio(4.0)
            .withOrientation(SplitPane::Orientation::VERTICAL)
            .withFixedPane(SplitPane::FixedPane::SECOND)
            .build();

        CHECK(splitPane->firstPane == firstPane);
        CHECK(splitPane->secondPane == secondPane);
        CHECK(splitPane->getGapWidth() == 0);
        CHECK(splitPane->getSecondPaneAspectRatio() == 4.0);
        CHECK(splitPane->getOrientation()
            == SplitPane::Orientation::VERTICAL);
    }
}

