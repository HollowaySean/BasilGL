#include <catch.hpp>

#include "Window/SplitPane.hpp"

#include "OpenGL/GLTestUtils.hpp"

using basil::PaneProps;
using basil::IPane;
using basil::SplitPane;
using basil::SplitPaneFixedPane;
using basil::SplitPaneOrientation;
using basil::SplitPaneOrientation::HORIZONTAL;
using basil::SplitPaneOrientation::VERTICAL;
using basil::Logger;
using basil::LogLevel;

template<class T>
using s_pt = std::shared_ptr<T>;

class TestPane : public IPane {
 public:
    explicit TestPane(PaneProps paneProps):
        IPane(paneProps) {}
    void const draw() override {
        didDraw = true;
    }
    bool didDraw = false;
};

TEST_CASE("Window_SplitPane_onResize") {
    SECTION("Sets props of pane") {
        SplitPane splitPane = SplitPane(testPaneProps);

        splitPane.onResize(10, 5);

        CHECK(splitPane.paneProps.width   == 10);
        CHECK(splitPane.paneProps.height  == 5);
        CHECK(splitPane.paneProps.xOffset == 5);
        CHECK(splitPane.paneProps.yOffset == 2);
    }
}

TEST_CASE("Window_SplitPane_setFirstPane") {
    auto childPane = std::make_shared<TestPane>(testPaneProps);
    auto secondChild = std::make_shared<TestPane>(testPaneProps);

    SECTION("Sets props of child pane for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPaneOrientation::HORIZONTAL);
        splitPane.setFirstPane(childPane);

        CHECK(childPane->paneProps.height == testPaneProps.height);
        CHECK(childPane->paneProps.width == testPaneProps.width / 2);
        CHECK(childPane->paneProps.xOffset == 5);
        CHECK(childPane->paneProps.yOffset == 2);
    }

    SECTION("Sets props of child pane for vertical orientation") {
        s_pt<SplitPane> splitPane = std::make_shared<SplitPane>(
            testPaneProps, SplitPaneOrientation::VERTICAL);
        splitPane->setFirstPane(childPane);

        CHECK(childPane->paneProps.height == testPaneProps.height / 2);
        CHECK(childPane->paneProps.width == testPaneProps.width);
        CHECK(childPane->paneProps.xOffset == 5);
        CHECK(childPane->paneProps.yOffset == 2);
    }

    SECTION("Logs warning if duplicate pane") {
        Logger& logger = Logger::get();

        s_pt<SplitPane> splitPane = std::make_shared<SplitPane>();
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
        s_pt<SplitPane> splitPane = std::make_shared<SplitPane>(
            testPaneProps, SplitPaneOrientation::HORIZONTAL);
        splitPane->setSecondPane(childPane);

        CHECK(childPane->paneProps.height == testPaneProps.height);
        CHECK(childPane->paneProps.width == testPaneProps.width / 2);
        CHECK(childPane->paneProps.xOffset ==
            testPaneProps.xOffset + testPaneProps.width / 2);
        CHECK(childPane->paneProps.yOffset == testPaneProps.yOffset);
    }

    SECTION("Sets props of child pane for vertical orientation") {
        s_pt<SplitPane> splitPane = std::make_shared<SplitPane>(
            testPaneProps, SplitPaneOrientation::VERTICAL);
        splitPane->setSecondPane(childPane);

        CHECK(childPane->paneProps.height == testPaneProps.height / 2);
        CHECK(childPane->paneProps.width == testPaneProps.width);
        CHECK(childPane->paneProps.xOffset == testPaneProps.xOffset);
        CHECK(childPane->paneProps.yOffset ==
            testPaneProps.yOffset + testPaneProps.height / 2);
    }

    SECTION("Logs warning if duplicate pane") {
        Logger& logger = Logger::get();

        s_pt<SplitPane> splitPane = std::make_shared<SplitPane>();
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

TEST_CASE("Window_SplitPane_draw") {
    s_pt<TestPane> firstPane = std::make_shared<TestPane>(testPaneProps);
    s_pt<TestPane> secondPane = std::make_shared<TestPane>(testPaneProps);

    SECTION("Calls draw on child panes") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPaneOrientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        CHECK(firstPane->didDraw == false);
        CHECK(secondPane->didDraw == false);

        splitPane.draw();

        CHECK(firstPane->didDraw == true);
        CHECK(secondPane->didDraw == true);
    }
}

TEST_CASE("Window_SplitPane_resizeToPixelValue") {
    s_pt<TestPane> firstPane = std::make_shared<TestPane>(testPaneProps);
    s_pt<TestPane> secondPane = std::make_shared<TestPane>(testPaneProps);

    SECTION("Updates sizes of child panes for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPaneOrientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.setPaneSizeInPixels(5);

        CHECK(firstPane->paneProps.width == 5);
        CHECK(firstPane->paneProps.height == 10);
        CHECK(firstPane->paneProps.xOffset == 5);
        CHECK(firstPane->paneProps.yOffset == 2);

        CHECK(secondPane->paneProps.width == 15);
        CHECK(secondPane->paneProps.height == 10);
        CHECK(secondPane->paneProps.xOffset == 10);
        CHECK(secondPane->paneProps.yOffset == 2);

        CHECK(splitPane.getFirstPaneSizeInPixels() == 5);
        CHECK(splitPane.getSecondPaneSizeInPixels() == 15);
    }

    SECTION("Updates sizes of child panes for vertical orientation") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPaneOrientation::VERTICAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.setPaneSizeInPixels(2);

        CHECK(firstPane->paneProps.width == 20);
        CHECK(firstPane->paneProps.height == 2);
        CHECK(firstPane->paneProps.xOffset == 5);
        CHECK(firstPane->paneProps.yOffset == 2);

        CHECK(secondPane->paneProps.width == 20);
        CHECK(secondPane->paneProps.height == 8);
        CHECK(secondPane->paneProps.xOffset == 5);
        CHECK(secondPane->paneProps.yOffset == 4);

        CHECK(splitPane.getFirstPaneSizeInPixels() == 2);
        CHECK(splitPane.getSecondPaneSizeInPixels() == 8);
    }
}

TEST_CASE("Window_SplitPane_resizeToPercentage") {
    s_pt<TestPane> firstPane = std::make_shared<TestPane>(testPaneProps);
    s_pt<TestPane> secondPane = std::make_shared<TestPane>(testPaneProps);

    SECTION("Updates sizes of child panes for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPaneOrientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.setPaneSizeAsPercentage(25.f);

        CHECK(firstPane->paneProps.width == 5);
        CHECK(firstPane->paneProps.height == 10);
        CHECK(firstPane->paneProps.xOffset == 5);
        CHECK(firstPane->paneProps.yOffset == 2);

        CHECK(secondPane->paneProps.width == 15);
        CHECK(secondPane->paneProps.height == 10);
        CHECK(secondPane->paneProps.xOffset == 10);
        CHECK(secondPane->paneProps.yOffset == 2);

        CHECK(splitPane.getFirstPaneSizeInPixels() == 5);
        CHECK(splitPane.getSecondPaneSizeInPixels() == 15);
    }

    SECTION("Updates sizes of child panes for vertical orientation") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPaneOrientation::VERTICAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.setPaneSizeAsPercentage(25.f);

        CHECK(firstPane->paneProps.width == 20);
        CHECK(firstPane->paneProps.height == 2);
        CHECK(firstPane->paneProps.xOffset == 5);
        CHECK(firstPane->paneProps.yOffset == 2);

        CHECK(secondPane->paneProps.width == 20);
        CHECK(secondPane->paneProps.height == 8);
        CHECK(secondPane->paneProps.xOffset == 5);
        CHECK(secondPane->paneProps.yOffset == 4);

        CHECK(splitPane.getFirstPaneSizeInPixels() == 2);
        CHECK(splitPane.getSecondPaneSizeInPixels() == 8);
    }

    SECTION("Clamps value if extent out of bounds") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPaneOrientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.setPaneSizeAsPercentage(-5.f);
        CHECK(firstPane->paneProps.width == 0);

        splitPane.setPaneSizeAsPercentage(120.f);
        CHECK(firstPane->paneProps.width == testPaneProps.width);
    }
}

TEST_CASE("Window_SplitPane_setGapWidth") {
    s_pt<TestPane> firstPane = std::make_shared<TestPane>(testPaneProps);
    s_pt<TestPane> secondPane = std::make_shared<TestPane>(testPaneProps);

    SECTION("Resizes to include gap, for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPaneOrientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.setGapWidth(4);

        CHECK(firstPane->paneProps.width == 10);
        CHECK(firstPane->paneProps.height == 10);
        CHECK(firstPane->paneProps.xOffset == 5);
        CHECK(firstPane->paneProps.yOffset == 2);

        CHECK(secondPane->paneProps.width == 6);
        CHECK(secondPane->paneProps.height == 10);
        CHECK(secondPane->paneProps.xOffset == 19);
        CHECK(secondPane->paneProps.yOffset == 2);
    }

    SECTION("Resizes to include gap, for vertical orientation") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPaneOrientation::VERTICAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.setGapWidth(4);

        CHECK(firstPane->paneProps.width == 20);
        CHECK(firstPane->paneProps.height == 5);
        CHECK(firstPane->paneProps.xOffset == 5);
        CHECK(firstPane->paneProps.yOffset == 2);

        CHECK(secondPane->paneProps.width == 20);
        CHECK(secondPane->paneProps.height == 1);
        CHECK(secondPane->paneProps.xOffset == 5);
        CHECK(secondPane->paneProps.yOffset == 11);
    }
}

TEST_CASE("Window_SplitPane_setOrientation") {
    SECTION("Resizes to include gap") {
        SplitPane splitPane = SplitPane(
            testPaneProps, SplitPaneOrientation::HORIZONTAL);

        splitPane.setOrientation(SplitPaneOrientation::VERTICAL);

        CHECK(splitPane.getOrientation() == SplitPaneOrientation::VERTICAL);
    }
}

TEST_CASE("Window_SplitPane_Builder") {
    s_pt<TestPane> firstPane = std::make_shared<TestPane>(testPaneProps);
    s_pt<TestPane> secondPane = std::make_shared<TestPane>(testPaneProps);

    auto paneProps = PaneProps {
        200, 100, 5, 10
    };

    SECTION("Builds SplitPane object with set percentage") {
        auto splitPane = SplitPane::Builder()
            .withPaneProps(paneProps)
            .withFirstPane(firstPane)
            .withSecondPane(secondPane)
            .withPaneExtentInPercent(30.)
            .withGapWidth(3)
            .withOrientation(SplitPaneOrientation::VERTICAL)
            .build();

        CHECK(splitPane->firstPane == firstPane);
        CHECK(splitPane->secondPane == secondPane);
        CHECK(splitPane->getGapWidth() == 3);
        CHECK(splitPane->getFirstPaneSizeAsPercentage() == 30.);
        CHECK(splitPane->getOrientation() == SplitPaneOrientation::VERTICAL);
    }


    SECTION("Builds SplitPane object with set pixels") {
        auto splitPane = SplitPane::Builder()
            .withPaneProps(paneProps)
            .withPaneExtentInPixels(15)
            .withOrientation(SplitPaneOrientation::HORIZONTAL)
            .build();

        CHECK(splitPane->firstPane == nullptr);
        CHECK(splitPane->secondPane == nullptr);
        CHECK(splitPane->getGapWidth() == 0);
        CHECK(splitPane->getFirstPaneSizeInPixels() == 15);
        CHECK(splitPane->getOrientation() == SplitPaneOrientation::HORIZONTAL);
    }


    SECTION("Builds SplitPane object with set aspect ratio") {
        auto splitPane = SplitPane::Builder()
            .withPaneProps(paneProps)
            .withOrientation(SplitPaneOrientation::VERTICAL)
            .withFixedPane(SplitPaneFixedPane::SECOND)
            .withPaneAspectRatio(4.0)
            .build();

        CHECK(splitPane->firstPane == nullptr);
        CHECK(splitPane->secondPane == nullptr);
        CHECK(splitPane->getGapWidth() == 0);
        CHECK(splitPane->getSecondPaneSizeInPixels() == paneProps.width / 4);
        CHECK(splitPane->getOrientation() == SplitPaneOrientation::VERTICAL);
    }
}
