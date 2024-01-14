#include "SplitPane.hpp"

#include <catch.hpp>

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

TEST_CASE("Window_SplitPane_setFirstPane") {
    TestPane childPane = TestPane(paneProps);

    SECTION("Sets props of child pane for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::HORIZONTAL);
        splitPane.setFirstPane(&childPane);

        REQUIRE(childPane.paneProps.height == paneProps.height);
        REQUIRE(childPane.paneProps.width == paneProps.width / 2);
        REQUIRE(childPane.paneProps.xOffset == 5);
        REQUIRE(childPane.paneProps.yOffset == 2);
    }

    SECTION("Sets props of child pane for vertical orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::VERTICAL);
        splitPane.setFirstPane(&childPane);

        REQUIRE(childPane.paneProps.height == paneProps.height / 2);
        REQUIRE(childPane.paneProps.width == paneProps.width);
        REQUIRE(childPane.paneProps.xOffset == 5);
        REQUIRE(childPane.paneProps.yOffset == 2);
    }
}

TEST_CASE("Window_SplitPane_setSecondPane") {
    TestPane childPane = TestPane(paneProps);

    SECTION("Sets props of child pane for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::HORIZONTAL);
        splitPane.setSecondPane(&childPane);

        REQUIRE(childPane.paneProps.height == paneProps.height);
        REQUIRE(childPane.paneProps.width == paneProps.width / 2);
        REQUIRE(childPane.paneProps.xOffset == paneProps.xOffset + paneProps.width / 2);
        REQUIRE(childPane.paneProps.yOffset == paneProps.yOffset);
    }

    SECTION("Sets props of child pane for vertical orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::VERTICAL);
        splitPane.setSecondPane(&childPane);

        REQUIRE(childPane.paneProps.height == paneProps.height / 2);
        REQUIRE(childPane.paneProps.width == paneProps.width);
        REQUIRE(childPane.paneProps.xOffset == paneProps.xOffset);
        REQUIRE(childPane.paneProps.yOffset == paneProps.yOffset + paneProps.height / 2);
    }
}

TEST_CASE("Window_SplitPane_draw") {
    TestPane firstPane = TestPane(paneProps);
    TestPane secondPane = TestPane(paneProps);

    SECTION("Calls draw on child panes") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::HORIZONTAL);

        splitPane.setFirstPane(&firstPane);
        splitPane.setSecondPane(&secondPane);

        REQUIRE(firstPane.didDraw == false);
        REQUIRE(secondPane.didDraw == false);

        splitPane.draw();

        REQUIRE(firstPane.didDraw == true);
        REQUIRE(secondPane.didDraw == true);
    }
}

TEST_CASE("Window_SplitPane_resizeToPixelValue") {
    TestPane firstPane = TestPane(paneProps);
    TestPane secondPane = TestPane(paneProps);

    SECTION("Updates sizes of child panes for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::HORIZONTAL);

        splitPane.setFirstPane(&firstPane);
        splitPane.setSecondPane(&secondPane);

        splitPane.resizeToPixelValue(5);

        REQUIRE(firstPane.paneProps.width == 5);
        REQUIRE(firstPane.paneProps.height == 10);
        REQUIRE(firstPane.paneProps.xOffset == 5);
        REQUIRE(firstPane.paneProps.yOffset == 2);

        REQUIRE(secondPane.paneProps.width == 15);
        REQUIRE(secondPane.paneProps.height == 10);
        REQUIRE(secondPane.paneProps.xOffset == 10);
        REQUIRE(secondPane.paneProps.yOffset == 2);

        REQUIRE(splitPane.getFirstPaneSizeInPixels() == 5);
        REQUIRE(splitPane.getSecondPaneSizeInPixels() == 15);
    }

    SECTION("Updates sizes of child panes for vertical orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::VERTICAL);

        splitPane.setFirstPane(&firstPane);
        splitPane.setSecondPane(&secondPane);

        splitPane.resizeToPixelValue(2);

        REQUIRE(firstPane.paneProps.width == 20);
        REQUIRE(firstPane.paneProps.height == 2);
        REQUIRE(firstPane.paneProps.xOffset == 5);
        REQUIRE(firstPane.paneProps.yOffset == 2);

        REQUIRE(secondPane.paneProps.width == 20);
        REQUIRE(secondPane.paneProps.height == 8);
        REQUIRE(secondPane.paneProps.xOffset == 5);
        REQUIRE(secondPane.paneProps.yOffset == 4);

        REQUIRE(splitPane.getFirstPaneSizeInPixels() == 2);
        REQUIRE(splitPane.getSecondPaneSizeInPixels() == 8);
    }
}

TEST_CASE("Window_SplitPane_resizeToPercentage") {
    TestPane firstPane = TestPane(paneProps);
    TestPane secondPane = TestPane(paneProps);

    SECTION("Updates sizes of child panes for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::HORIZONTAL);

        splitPane.setFirstPane(&firstPane);
        splitPane.setSecondPane(&secondPane);

        splitPane.resizeToPercentage(25.f);

        REQUIRE(firstPane.paneProps.width == 5);
        REQUIRE(firstPane.paneProps.height == 10);
        REQUIRE(firstPane.paneProps.xOffset == 5);
        REQUIRE(firstPane.paneProps.yOffset == 2);

        REQUIRE(secondPane.paneProps.width == 15);
        REQUIRE(secondPane.paneProps.height == 10);
        REQUIRE(secondPane.paneProps.xOffset == 10);
        REQUIRE(secondPane.paneProps.yOffset == 2);

        REQUIRE(splitPane.getFirstPaneSizeInPixels() == 5);
        REQUIRE(splitPane.getSecondPaneSizeInPixels() == 15);
    }

    SECTION("Updates sizes of child panes for vertical orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::VERTICAL);

        splitPane.setFirstPane(&firstPane);
        splitPane.setSecondPane(&secondPane);

        splitPane.resizeToPercentage(25.f);

        REQUIRE(firstPane.paneProps.width == 20);
        REQUIRE(firstPane.paneProps.height == 2);
        REQUIRE(firstPane.paneProps.xOffset == 5);
        REQUIRE(firstPane.paneProps.yOffset == 2);

        REQUIRE(secondPane.paneProps.width == 20);
        REQUIRE(secondPane.paneProps.height == 8);
        REQUIRE(secondPane.paneProps.xOffset == 5);
        REQUIRE(secondPane.paneProps.yOffset == 4);

        REQUIRE(splitPane.getFirstPaneSizeInPixels() == 2);
        REQUIRE(splitPane.getSecondPaneSizeInPixels() == 8);
    }

    SECTION("Does nothing if extent out of bounds") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::HORIZONTAL);

        splitPane.setFirstPane(&firstPane);
        splitPane.setSecondPane(&secondPane);

        splitPane.resizeToPercentage(-5.f);
        REQUIRE(firstPane.paneProps.width == 10);

        splitPane.resizeToPercentage(120.f);
        REQUIRE(firstPane.paneProps.width == 10);
    }
}

TEST_CASE("Window_SplitPane_setGapWidth") {
    TestPane firstPane = TestPane(paneProps);
    TestPane secondPane = TestPane(paneProps);

    SECTION("Resizes to include gap, for horizontal orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::HORIZONTAL);

        splitPane.setFirstPane(&firstPane);
        splitPane.setSecondPane(&secondPane);

        splitPane.setGapWidth(4);

        REQUIRE(firstPane.paneProps.width == 8);
        REQUIRE(firstPane.paneProps.height == 10);
        REQUIRE(firstPane.paneProps.xOffset == 5);
        REQUIRE(firstPane.paneProps.yOffset == 2);

        REQUIRE(secondPane.paneProps.width == 8);
        REQUIRE(secondPane.paneProps.height == 10);
        REQUIRE(secondPane.paneProps.xOffset == 17);
        REQUIRE(secondPane.paneProps.yOffset == 2);
    }

    SECTION("Resizes to include gap, for vertical orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::VERTICAL);

        splitPane.setFirstPane(&firstPane);
        splitPane.setSecondPane(&secondPane);

        splitPane.setGapWidth(4);

        REQUIRE(firstPane.paneProps.width == 20);
        REQUIRE(firstPane.paneProps.height == 3);
        REQUIRE(firstPane.paneProps.xOffset == 5);
        REQUIRE(firstPane.paneProps.yOffset == 2);

        REQUIRE(secondPane.paneProps.width == 20);
        REQUIRE(secondPane.paneProps.height == 3);
        REQUIRE(secondPane.paneProps.xOffset == 5);
        REQUIRE(secondPane.paneProps.yOffset == 9);
    }
}

TEST_CASE("Window_SplitPane_setOrientation") {
    SECTION("Resizes to include gap") {
        SplitPane splitPane = SplitPane(
            paneProps, HORIZONTAL);

        splitPane.setOrientation(VERTICAL);

        REQUIRE(splitPane.getOrientation() == VERTICAL);
    }
}
