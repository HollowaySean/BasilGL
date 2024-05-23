#include <catch.hpp>

#include "Window/SplitPane.hpp"

#include "Window/WindowTestUtils.hpp"

using basil::ViewArea;
using basil::IPane;
using basil::SplitPane;
using basil::Logger;
using basil::LogLevel;

TEST_CASE("Window_SplitPane_draw") {
    auto firstPane = std::make_shared<TestPane>(testViewArea);
    auto secondPane = std::make_shared<TestPane>(testViewArea);

    SECTION("Calls draw on child panes") {
        SplitPane splitPane = SplitPane(
            testViewArea, SplitPane::Orientation::HORIZONTAL);

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
        SplitPane splitPane = SplitPane(testViewArea);

        splitPane.onResize(10, 5);

        CHECK(splitPane.viewArea.width   == 10);
        CHECK(splitPane.viewArea.height  == 5);
        CHECK(splitPane.viewArea.xOffset == testViewArea.xOffset);
        CHECK(splitPane.viewArea.yOffset == testViewArea.yOffset);
    }

    SECTION("Resizes split") {
        SplitPane splitPane = SplitPane(testViewArea);
        int setExtent = 2;
        splitPane.setPaneSizeInPixels(setExtent);

        CHECK(splitPane.firstPaneExtent == setExtent);
        CHECK(splitPane.secondPaneExtent == testViewArea.width - setExtent);

        splitPane.onResize(10, 5);

        CHECK(splitPane.firstPaneExtent == setExtent);
        CHECK(splitPane.secondPaneExtent == 10 - setExtent);
    }
}

TEST_CASE("Window_SplitPane_setFirstPane") {
    auto childPane = std::make_shared<TestPane>(testViewArea);
    auto secondChild = std::make_shared<TestPane>(testViewArea);

    SECTION("Sets props of child pane for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            testViewArea, SplitPane::Orientation::HORIZONTAL);
        splitPane.setFirstPane(childPane);

        CHECK(childPane->viewArea.height == testViewArea.height);
        CHECK(childPane->viewArea.width == testViewArea.width / 2);
        CHECK(childPane->viewArea.xOffset == testViewArea.xOffset);
        CHECK(childPane->viewArea.yOffset == testViewArea.yOffset);
    }

    SECTION("Sets props of child pane for vertical orientation") {
        auto splitPane = std::make_shared<SplitPane>(
            testViewArea, SplitPane::Orientation::VERTICAL);
        splitPane->setFirstPane(childPane);

        CHECK(childPane->viewArea.height == testViewArea.height / 2);
        CHECK(childPane->viewArea.width == testViewArea.width);
        CHECK(childPane->viewArea.xOffset == testViewArea.xOffset);
        CHECK(childPane->viewArea.yOffset == testViewArea.yOffset);
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
    auto childPane = std::make_shared<TestPane>(testViewArea);
    auto secondChild = std::make_shared<TestPane>(testViewArea);

    SECTION("Sets props of child pane for horizontal orientation") {
        auto splitPane = std::make_shared<SplitPane>(
            testViewArea, SplitPane::Orientation::HORIZONTAL);
        splitPane->setSecondPane(childPane);

        CHECK(childPane->viewArea.height == testViewArea.height);
        CHECK(childPane->viewArea.width == testViewArea.width / 2);
        CHECK(childPane->viewArea.xOffset ==
            testViewArea.xOffset + testViewArea.width / 2);
        CHECK(childPane->viewArea.yOffset == testViewArea.yOffset);
    }

    SECTION("Sets props of child pane for vertical orientation") {
        auto splitPane = std::make_shared<SplitPane>(
            testViewArea, SplitPane::Orientation::VERTICAL);
        splitPane->setSecondPane(childPane);

        CHECK(childPane->viewArea.height == testViewArea.height / 2);
        CHECK(childPane->viewArea.width == testViewArea.width);
        CHECK(childPane->viewArea.xOffset == testViewArea.xOffset);
        CHECK(childPane->viewArea.yOffset ==
            testViewArea.yOffset + testViewArea.height / 2);
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
    auto splitPane = SplitPane(testViewArea);
    splitPane.setPaneSizeInPixels(30);

    SECTION("Sets fixed pane and resizes") {
        CHECK(splitPane.getFirstPaneSizeInPixels() == 30);
        splitPane.setFixedPane(SplitPane::FixedPane::SECOND);
        CHECK(splitPane.getSecondPaneSizeInPixels() == 30);
    }
}

TEST_CASE("Window_SplitPane_setPaneSizeInPixels") {
    auto firstPane = std::make_shared<TestPane>(testViewArea);
    auto secondPane = std::make_shared<TestPane>(testViewArea);

    SECTION("Updates sizes of child panes for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            testViewArea, SplitPane::Orientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        int paneSize = 50;
        splitPane.setPaneSizeInPixels(paneSize);

        CHECK(firstPane->viewArea.width
            == paneSize);
        CHECK(firstPane->viewArea.height
            == testViewArea.height);
        CHECK(firstPane->viewArea.xOffset
            == testViewArea.xOffset);
        CHECK(firstPane->viewArea.yOffset
            == testViewArea.yOffset);

        CHECK(secondPane->viewArea.width
            == testViewArea.width - paneSize);
        CHECK(secondPane->viewArea.height
            == testViewArea.height);
        CHECK(secondPane->viewArea.xOffset
            == testViewArea.xOffset + paneSize);
        CHECK(secondPane->viewArea.yOffset
            == testViewArea.yOffset);

        CHECK(splitPane.getFirstPaneSizeInPixels()
            == paneSize);
        CHECK(splitPane.getSecondPaneSizeInPixels()
            == testViewArea.width - paneSize);
    }

    SECTION("Updates sizes of child panes for vertical orientation") {
        SplitPane splitPane = SplitPane(
            testViewArea, SplitPane::Orientation::VERTICAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        int paneSize = 50;
        splitPane.setPaneSizeInPixels(paneSize);

        CHECK(firstPane->viewArea.width
            == testViewArea.width);
        CHECK(firstPane->viewArea.height
            == paneSize);
        CHECK(firstPane->viewArea.xOffset
            == testViewArea.xOffset);
        CHECK(firstPane->viewArea.yOffset
            == testViewArea.yOffset);

        CHECK(secondPane->viewArea.width
            == testViewArea.width);
        CHECK(secondPane->viewArea.height
            == testViewArea.height - paneSize);
        CHECK(secondPane->viewArea.xOffset
            == testViewArea.xOffset);
        CHECK(secondPane->viewArea.yOffset
            == testViewArea.yOffset + paneSize);

        CHECK(splitPane.getFirstPaneSizeInPixels()
            == paneSize);
        CHECK(splitPane.getSecondPaneSizeInPixels()
            == testViewArea.height - paneSize);
    }

    SECTION("Clamps value if extent out of bounds") {
        SplitPane splitPane = SplitPane(
            testViewArea, SplitPane::Orientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.setPaneSizeInPixels(-5);
        CHECK(firstPane->viewArea.width == 0);

        CHECK(splitPane.getFirstPaneSizeInPixels()
            == 0.);
        CHECK(splitPane.getSecondPaneSizeInPixels()
            == testViewArea.width);

        splitPane.setPaneSizeInPixels(10000.f);
        CHECK(firstPane->viewArea.width == testViewArea.width);

        CHECK(splitPane.getFirstPaneSizeInPixels()
            == testViewArea.width);
        CHECK(splitPane.getSecondPaneSizeInPixels()
            == 0.);
    }
}

TEST_CASE("Window_SplitPane_setPaneSizeAsPercentage") {
    auto firstPane = std::make_shared<TestPane>(testViewArea);
    auto secondPane = std::make_shared<TestPane>(testViewArea);

    SECTION("Updates sizes of child panes for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            testViewArea, SplitPane::Orientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        float setPercentage = 25.f;
        splitPane.setPaneSizeAsPercentage(setPercentage);

        int paneSize = testViewArea.width * (setPercentage / 100.);

        CHECK(firstPane->viewArea.width
            == paneSize);
        CHECK(firstPane->viewArea.height
            == testViewArea.height);
        CHECK(firstPane->viewArea.xOffset
            == testViewArea.xOffset);
        CHECK(firstPane->viewArea.yOffset
            == testViewArea.yOffset);

        CHECK(secondPane->viewArea.width
            == testViewArea.width - paneSize);
        CHECK(secondPane->viewArea.height
            == testViewArea.height);
        CHECK(secondPane->viewArea.xOffset
            == testViewArea.xOffset + paneSize);
        CHECK(secondPane->viewArea.yOffset
            == testViewArea.yOffset);

        CHECK(splitPane.getFirstPaneSizeAsPercentage()
            == setPercentage);
        CHECK(splitPane.getSecondPaneSizeAsPercentage()
            == 100. - setPercentage);
    }

    SECTION("Updates sizes of child panes for vertical orientation") {
        SplitPane splitPane = SplitPane(
            testViewArea, SplitPane::Orientation::VERTICAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        float setPercentage = 25.f;
        splitPane.setPaneSizeAsPercentage(setPercentage);

        int paneSize = testViewArea.height * (setPercentage / 100.);

        CHECK(firstPane->viewArea.width
            == testViewArea.width);
        CHECK(firstPane->viewArea.height
            == paneSize);
        CHECK(firstPane->viewArea.xOffset
            == testViewArea.xOffset);
        CHECK(firstPane->viewArea.yOffset
            == testViewArea.yOffset);

        CHECK(secondPane->viewArea.width
            == testViewArea.width);
        CHECK(secondPane->viewArea.height
            == testViewArea.height - paneSize);
        CHECK(secondPane->viewArea.xOffset
            == testViewArea.xOffset);
        CHECK(secondPane->viewArea.yOffset
            == testViewArea.yOffset + paneSize);

        CHECK(splitPane.getFirstPaneSizeAsPercentage()
            == setPercentage);
        CHECK(splitPane.getSecondPaneSizeAsPercentage()
            == 100. - setPercentage);
    }

    SECTION("Clamps value if extent out of bounds") {
        SplitPane splitPane = SplitPane(
            testViewArea, SplitPane::Orientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.setPaneSizeAsPercentage(-5.f);
        CHECK(firstPane->viewArea.width == 0);

        CHECK(splitPane.getFirstPaneSizeAsPercentage() == 0.);
        CHECK(splitPane.getSecondPaneSizeAsPercentage() == 100.);

        splitPane.setPaneSizeAsPercentage(120.f);
        CHECK(firstPane->viewArea.width == testViewArea.width);

        CHECK(splitPane.getFirstPaneSizeAsPercentage() == 100.);
        CHECK(splitPane.getSecondPaneSizeAsPercentage() == 0.);
    }
}

TEST_CASE("Window_SplitPane_setPaneAspectRatio") {
    auto firstPane = std::make_shared<TestPane>(testViewArea);
    auto secondPane = std::make_shared<TestPane>(testViewArea);

    SECTION("Updates sizes of child panes for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            testViewArea, SplitPane::Orientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        float setRatio = 1.25f;
        splitPane.setPaneAspectRatio(setRatio);

        int paneSize = testViewArea.height * setRatio;

        CHECK(firstPane->viewArea.width
            == paneSize);
        CHECK(firstPane->viewArea.height
            == testViewArea.height);
        CHECK(firstPane->viewArea.xOffset
            == testViewArea.xOffset);
        CHECK(firstPane->viewArea.yOffset
            == testViewArea.yOffset);

        CHECK(secondPane->viewArea.width
            == testViewArea.width - paneSize);
        CHECK(secondPane->viewArea.height
            == testViewArea.height);
        CHECK(secondPane->viewArea.xOffset
            == testViewArea.xOffset + paneSize);
        CHECK(secondPane->viewArea.yOffset
            == testViewArea.yOffset);

        CHECK(splitPane.getFirstPaneAspectRatio()
            == setRatio);
        CHECK(splitPane.getSecondPaneAspectRatio()
            == static_cast<float>((testViewArea.width - paneSize))
                / testViewArea.height);
    }

    SECTION("Updates sizes of child panes for vertical orientation") {
        SplitPane splitPane = SplitPane(
            testViewArea, SplitPane::Orientation::VERTICAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        float setRatio = 2.5f;
        splitPane.setPaneAspectRatio(setRatio);

        int paneSize = testViewArea.width / setRatio;

        CHECK(firstPane->viewArea.width
            == testViewArea.width);
        CHECK(firstPane->viewArea.height
            == paneSize);
        CHECK(firstPane->viewArea.xOffset
            == testViewArea.xOffset);
        CHECK(firstPane->viewArea.yOffset
            == testViewArea.yOffset);

        CHECK(secondPane->viewArea.width
            == testViewArea.width);
        CHECK(secondPane->viewArea.height
            == testViewArea.height - paneSize);
        CHECK(secondPane->viewArea.xOffset
            == testViewArea.xOffset);
        CHECK(secondPane->viewArea.yOffset
            == testViewArea.yOffset + paneSize);

        CHECK(splitPane.getFirstPaneAspectRatio()
            == setRatio);
        CHECK(splitPane.getSecondPaneAspectRatio()
            == static_cast<float>(testViewArea.width
                / (testViewArea.height - paneSize)));
    }

    SECTION("Clamps value if extent out of bounds") {
        SplitPane splitPane = SplitPane(
            testViewArea, SplitPane::Orientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.setPaneAspectRatio(-5.f);
        CHECK(firstPane->viewArea.width == 0);

        CHECK(splitPane.getFirstPaneAspectRatio() == 0);

        splitPane.setPaneAspectRatio(100.f);
        CHECK(firstPane->viewArea.width == testViewArea.width);

        CHECK(splitPane.getFirstPaneAspectRatio()
            == splitPane.getTotalAspect());
    }
}

TEST_CASE("Window_SplitPane_setGapWidth") {
    auto firstPane = std::make_shared<TestPane>(testViewArea);
    auto secondPane = std::make_shared<TestPane>(testViewArea);

    SECTION("Resizes to include gap, for horizontal orientation") {
        SplitPane splitPane = SplitPane(testViewArea);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        int paneSize = 50;
        splitPane.setPaneSizeInPixels(paneSize);

        int gapWidth = 4;
        splitPane.setGapWidth(gapWidth);

        CHECK(splitPane.getGapWidth() == gapWidth);

        CHECK(firstPane->viewArea.width
            == paneSize);
        CHECK(firstPane->viewArea.height
            == testViewArea.height);
        CHECK(firstPane->viewArea.xOffset
            == testViewArea.xOffset);
        CHECK(firstPane->viewArea.yOffset
            == testViewArea.yOffset);

        CHECK(secondPane->viewArea.width
            == testViewArea.width - paneSize - gapWidth);
        CHECK(secondPane->viewArea.height
            == testViewArea.height);
        CHECK(secondPane->viewArea.xOffset
            == testViewArea.xOffset + paneSize + gapWidth);
        CHECK(secondPane->viewArea.yOffset
            == testViewArea.yOffset);
    }
}

TEST_CASE("Window_SplitPane_setOrientation") {
    SECTION("Sets pane orientation") {
        SplitPane splitPane = SplitPane(
            testViewArea, SplitPane::Orientation::HORIZONTAL);

        splitPane.setOrientation(SplitPane::Orientation::VERTICAL);

        CHECK(splitPane.getOrientation() == SplitPane::Orientation::VERTICAL);
    }
}

TEST_CASE("Window_SplitPane_Builder") {
    auto firstPane = std::make_shared<TestPane>(testViewArea);
    auto secondPane = std::make_shared<TestPane>(testViewArea);

    SECTION("Builds SplitPane object with set percentage") {
        auto splitPane = SplitPane::Builder()
            .withPaneProps(testViewArea)
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
            .withPaneProps(testViewArea)
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
            .withPaneProps(testViewArea)
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

