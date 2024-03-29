#include <catch.hpp>

#include <Basil/Window.hpp>

using basil::PaneProps;
using basil::IPane;
using basil::SplitPane;
using basil::PaneOrientation;
using basil::PaneOrientation::HORIZONTAL;
using basil::PaneOrientation::VERTICAL;
using basil::Logger;
using basil::LogLevel;

template<class T>
using s_pt = std::shared_ptr<T>;

PaneProps paneProps = {
    .width = 20,
    .height = 10,
    .xOffset = 5,
    .yOffset = 2
};

class TestPane : public IPane {
 public:
    explicit TestPane(PaneProps paneProps):
        IPane(paneProps) {}
    void const draw() override {
        didDraw = true;
    }
    bool didDraw = false;
};

TEST_CASE("Window_SplitPane_SplitPane") {
    SECTION("Initializes with defaults") {
        SplitPane pane = SplitPane();
        PaneProps defaultProps = PaneProps();

        REQUIRE(pane.getGapWidth() == 0);
        REQUIRE(pane.getFirstPaneSizeAsPercentage() == 50.);

        REQUIRE(pane.paneProps.width    == defaultProps.width);
        REQUIRE(pane.paneProps.height   == defaultProps.height);
        REQUIRE(pane.paneProps.xOffset  == defaultProps.xOffset);
        REQUIRE(pane.paneProps.yOffset  == defaultProps.yOffset);
    }
}

TEST_CASE("Window_SplitPane_onResize") {
    SECTION("Sets props of pane") {
        SplitPane splitPane = SplitPane(paneProps);

        splitPane.onResize(10, 5);

        REQUIRE(splitPane.paneProps.width   == 10);
        REQUIRE(splitPane.paneProps.height  == 5);
        REQUIRE(splitPane.paneProps.xOffset == 5);
        REQUIRE(splitPane.paneProps.yOffset == 2);
    }

    SECTION("Maintains relative extent for horizontal layout") {
        SplitPane splitPane = SplitPane(paneProps, PaneOrientation::HORIZONTAL);

        TestPane firstPane = TestPane(paneProps);
        TestPane secondPane = TestPane(paneProps);

        splitPane.resizeToPixelValue(5);

        float firstPercentage =
            splitPane.getFirstPaneSizeAsPercentage();
        float secondPercentage =
            splitPane.getSecondPaneSizeAsPercentage();

        splitPane.onResize(200, 100);

        REQUIRE(splitPane.getFirstPaneSizeAsPercentage() ==
            firstPercentage);
        REQUIRE(splitPane.getSecondPaneSizeAsPercentage() ==
            secondPercentage);
    }

    SECTION("Maintains relative extent for vertical layout") {
        SplitPane splitPane = SplitPane(paneProps, PaneOrientation::VERTICAL);

        TestPane firstPane = TestPane(paneProps);
        TestPane secondPane = TestPane(paneProps);

        splitPane.resizeToPixelValue(2);

        float firstPercentage =
            splitPane.getFirstPaneSizeAsPercentage();
        float secondPercentage =
            splitPane.getSecondPaneSizeAsPercentage();

        splitPane.onResize(200, 100);

        REQUIRE(splitPane.getFirstPaneSizeAsPercentage() ==
            firstPercentage);
        REQUIRE(splitPane.getSecondPaneSizeAsPercentage() ==
            secondPercentage);
    }
}

TEST_CASE("Window_SplitPane_setFirstPane") {
    s_pt<TestPane> childPane = std::make_shared<TestPane>(paneProps);

    SECTION("Sets props of child pane for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::HORIZONTAL);
        splitPane.setFirstPane(childPane);

        REQUIRE(childPane->paneProps.height == paneProps.height);
        REQUIRE(childPane->paneProps.width == paneProps.width / 2);
        REQUIRE(childPane->paneProps.xOffset == 5);
        REQUIRE(childPane->paneProps.yOffset == 2);
    }

    SECTION("Sets props of child pane for vertical orientation") {
        s_pt<SplitPane> splitPane = std::make_shared<SplitPane>(
            paneProps, PaneOrientation::VERTICAL);
        splitPane->setFirstPane(childPane);

        REQUIRE(childPane->paneProps.height == paneProps.height / 2);
        REQUIRE(childPane->paneProps.width == paneProps.width);
        REQUIRE(childPane->paneProps.xOffset == 5);
        REQUIRE(childPane->paneProps.yOffset == 2);
    }

    SECTION("Logs warning if duplicate pane") {
        Logger& logger = Logger::get();

        s_pt<SplitPane> splitPane = std::make_shared<SplitPane>();
        splitPane->setSecondPane(childPane);
        splitPane->setFirstPane(childPane);

        REQUIRE(splitPane->firstPane == nullptr);
        REQUIRE(logger.getLastOutput() ==
            "First pane has same address as second. Pane not set.");
        REQUIRE(logger.getLastLevel() == LogLevel::WARN);
    }
}

TEST_CASE("Window_SplitPane_setSecondPane") {
    s_pt<TestPane> childPane = std::make_shared<TestPane>(paneProps);

    SECTION("Sets props of child pane for horizontal orientation") {
        s_pt<SplitPane> splitPane = std::make_shared<SplitPane>(
            paneProps, PaneOrientation::HORIZONTAL);
        splitPane->setSecondPane(childPane);

        REQUIRE(childPane->paneProps.height == paneProps.height);
        REQUIRE(childPane->paneProps.width == paneProps.width / 2);
        REQUIRE(childPane->paneProps.xOffset ==
            paneProps.xOffset + paneProps.width / 2);
        REQUIRE(childPane->paneProps.yOffset == paneProps.yOffset);
    }

    SECTION("Sets props of child pane for vertical orientation") {
        s_pt<SplitPane> splitPane = std::make_shared<SplitPane>(
            paneProps, PaneOrientation::VERTICAL);
        splitPane->setSecondPane(childPane);

        REQUIRE(childPane->paneProps.height == paneProps.height / 2);
        REQUIRE(childPane->paneProps.width == paneProps.width);
        REQUIRE(childPane->paneProps.xOffset == paneProps.xOffset);
        REQUIRE(childPane->paneProps.yOffset ==
            paneProps.yOffset + paneProps.height / 2);
    }

    SECTION("Logs warning if duplicate pane") {
        Logger& logger = Logger::get();

        s_pt<SplitPane> splitPane = std::make_shared<SplitPane>();
        splitPane->setFirstPane(childPane);
        splitPane->setSecondPane(childPane);

        REQUIRE(splitPane->secondPane == nullptr);
        REQUIRE(logger.getLastOutput() ==
            "Second pane has same address as first. Pane not set.");
        REQUIRE(logger.getLastLevel() == LogLevel::WARN);
    }
}

TEST_CASE("Window_SplitPane_draw") {
    s_pt<TestPane> firstPane = std::make_shared<TestPane>(paneProps);
    s_pt<TestPane> secondPane = std::make_shared<TestPane>(paneProps);

    SECTION("Calls draw on child panes") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        REQUIRE(firstPane->didDraw == false);
        REQUIRE(secondPane->didDraw == false);

        splitPane.draw();

        REQUIRE(firstPane->didDraw == true);
        REQUIRE(secondPane->didDraw == true);
    }
}

TEST_CASE("Window_SplitPane_resizeToPixelValue") {
    s_pt<TestPane> firstPane = std::make_shared<TestPane>(paneProps);
    s_pt<TestPane> secondPane = std::make_shared<TestPane>(paneProps);

    SECTION("Updates sizes of child panes for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.resizeToPixelValue(5);

        REQUIRE(firstPane->paneProps.width == 5);
        REQUIRE(firstPane->paneProps.height == 10);
        REQUIRE(firstPane->paneProps.xOffset == 5);
        REQUIRE(firstPane->paneProps.yOffset == 2);

        REQUIRE(secondPane->paneProps.width == 15);
        REQUIRE(secondPane->paneProps.height == 10);
        REQUIRE(secondPane->paneProps.xOffset == 10);
        REQUIRE(secondPane->paneProps.yOffset == 2);

        REQUIRE(splitPane.getFirstPaneSizeInPixels() == 5);
        REQUIRE(splitPane.getSecondPaneSizeInPixels() == 15);
    }

    SECTION("Updates sizes of child panes for vertical orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::VERTICAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.resizeToPixelValue(2);

        REQUIRE(firstPane->paneProps.width == 20);
        REQUIRE(firstPane->paneProps.height == 2);
        REQUIRE(firstPane->paneProps.xOffset == 5);
        REQUIRE(firstPane->paneProps.yOffset == 2);

        REQUIRE(secondPane->paneProps.width == 20);
        REQUIRE(secondPane->paneProps.height == 8);
        REQUIRE(secondPane->paneProps.xOffset == 5);
        REQUIRE(secondPane->paneProps.yOffset == 4);

        REQUIRE(splitPane.getFirstPaneSizeInPixels() == 2);
        REQUIRE(splitPane.getSecondPaneSizeInPixels() == 8);
    }
}

TEST_CASE("Window_SplitPane_resizeToPercentage") {
    s_pt<TestPane> firstPane = std::make_shared<TestPane>(paneProps);
    s_pt<TestPane> secondPane = std::make_shared<TestPane>(paneProps);

    SECTION("Updates sizes of child panes for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.resizeToPercentage(25.f);

        REQUIRE(firstPane->paneProps.width == 5);
        REQUIRE(firstPane->paneProps.height == 10);
        REQUIRE(firstPane->paneProps.xOffset == 5);
        REQUIRE(firstPane->paneProps.yOffset == 2);

        REQUIRE(secondPane->paneProps.width == 15);
        REQUIRE(secondPane->paneProps.height == 10);
        REQUIRE(secondPane->paneProps.xOffset == 10);
        REQUIRE(secondPane->paneProps.yOffset == 2);

        REQUIRE(splitPane.getFirstPaneSizeInPixels() == 5);
        REQUIRE(splitPane.getSecondPaneSizeInPixels() == 15);
    }

    SECTION("Updates sizes of child panes for vertical orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::VERTICAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.resizeToPercentage(25.f);

        REQUIRE(firstPane->paneProps.width == 20);
        REQUIRE(firstPane->paneProps.height == 2);
        REQUIRE(firstPane->paneProps.xOffset == 5);
        REQUIRE(firstPane->paneProps.yOffset == 2);

        REQUIRE(secondPane->paneProps.width == 20);
        REQUIRE(secondPane->paneProps.height == 8);
        REQUIRE(secondPane->paneProps.xOffset == 5);
        REQUIRE(secondPane->paneProps.yOffset == 4);

        REQUIRE(splitPane.getFirstPaneSizeInPixels() == 2);
        REQUIRE(splitPane.getSecondPaneSizeInPixels() == 8);
    }

    SECTION("Clamps value if extent out of bounds") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.resizeToPercentage(-5.f);
        REQUIRE(firstPane->paneProps.width == 0);

        splitPane.resizeToPercentage(120.f);
        REQUIRE(firstPane->paneProps.width == paneProps.width);
    }
}

TEST_CASE("Window_SplitPane_setGapWidth") {
    s_pt<TestPane> firstPane = std::make_shared<TestPane>(paneProps);
    s_pt<TestPane> secondPane = std::make_shared<TestPane>(paneProps);

    SECTION("Resizes to include gap, for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::HORIZONTAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.setGapWidth(4);

        REQUIRE(firstPane->paneProps.width == 8);
        REQUIRE(firstPane->paneProps.height == 10);
        REQUIRE(firstPane->paneProps.xOffset == 5);
        REQUIRE(firstPane->paneProps.yOffset == 2);

        REQUIRE(secondPane->paneProps.width == 8);
        REQUIRE(secondPane->paneProps.height == 10);
        REQUIRE(secondPane->paneProps.xOffset == 17);
        REQUIRE(secondPane->paneProps.yOffset == 2);
    }

    SECTION("Resizes to include gap, for vertical orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::VERTICAL);

        splitPane.setFirstPane(firstPane);
        splitPane.setSecondPane(secondPane);

        splitPane.setGapWidth(4);

        REQUIRE(firstPane->paneProps.width == 20);
        REQUIRE(firstPane->paneProps.height == 3);
        REQUIRE(firstPane->paneProps.xOffset == 5);
        REQUIRE(firstPane->paneProps.yOffset == 2);

        REQUIRE(secondPane->paneProps.width == 20);
        REQUIRE(secondPane->paneProps.height == 3);
        REQUIRE(secondPane->paneProps.xOffset == 5);
        REQUIRE(secondPane->paneProps.yOffset == 9);
    }
}

TEST_CASE("Window_SplitPane_setOrientation") {
    SECTION("Resizes to include gap") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::HORIZONTAL);

        splitPane.setOrientation(PaneOrientation::VERTICAL);

        REQUIRE(splitPane.getOrientation() == PaneOrientation::VERTICAL);
    }
}

TEST_CASE("Window_SplitPane_Builder") {
    s_pt<TestPane> firstPane = std::make_shared<TestPane>(paneProps);
    s_pt<TestPane> secondPane = std::make_shared<TestPane>(paneProps);

    SECTION("Builds SplitPane object") {
        auto splitPane = SplitPane::Builder()
            .withFirstPane(firstPane)
            .withSecondPane(secondPane)
            .withPaneExtentInPercent(30.)
            .withGapWidth(3)
            .withOrientation(PaneOrientation::VERTICAL)
            .build();

        REQUIRE(splitPane->firstPane == firstPane);
        REQUIRE(splitPane->secondPane == secondPane);
        REQUIRE(splitPane->gapWidth == 3);
        REQUIRE(splitPane->getFirstPaneSizeAsPercentage() == 30.);
        REQUIRE(splitPane->getOrientation() == PaneOrientation::VERTICAL);

        splitPane = SplitPane::Builder()
            .withPaneExtentInPixels(15)
            .build();

        REQUIRE(splitPane->firstPane == nullptr);
        REQUIRE(splitPane->secondPane == nullptr);
        REQUIRE(splitPane->gapWidth == 0);
        REQUIRE(splitPane->getFirstPaneSizeInPixels() == 15);
        REQUIRE(splitPane->getOrientation() == PaneOrientation::HORIZONTAL);
    }
}
