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
        REQUIRE(childPane.paneProps.xOffset == 0);
        REQUIRE(childPane.paneProps.yOffset == 0);
    }

    SECTION("Sets props of child pane for vertical orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::VERTICAL);
        splitPane.setFirstPane(&childPane);

        REQUIRE(childPane.paneProps.height == paneProps.height / 2);
        REQUIRE(childPane.paneProps.width == paneProps.width);
        REQUIRE(childPane.paneProps.xOffset == 0);
        REQUIRE(childPane.paneProps.yOffset == 0);
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
        REQUIRE(childPane.paneProps.xOffset == paneProps.width / 2);
        REQUIRE(childPane.paneProps.yOffset == 0);
    }

    SECTION("Sets props of child pane for vertical orientation") {
        SplitPane splitPane = SplitPane(
            paneProps, PaneOrientation::VERTICAL);
        splitPane.setSecondPane(&childPane);

        REQUIRE(childPane.paneProps.height == paneProps.height / 2);
        REQUIRE(childPane.paneProps.width == paneProps.width);
        REQUIRE(childPane.paneProps.xOffset == 0);
        REQUIRE(childPane.paneProps.yOffset == paneProps.height / 2);
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
