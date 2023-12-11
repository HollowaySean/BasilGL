#include "SplitPane.hpp"

void const SplitPane::draw() {
    if (firstPane) {
        firstPane->draw();
    }
    if (secondPane) {
        secondPane->draw();
    }
}

void SplitPane::setFirstPane(IPane* pane) {
    firstPane = pane;

    PaneProps newProps = PaneProps();

    switch (orientation) {
        case PaneOrientation::HORIZONTAL:
            newProps.height  = paneProps.height;
            newProps.width   = paneProps.width / 2;
            newProps.xOffset = paneProps.width / 2;
            newProps.yOffset = 0;
            break;
        case PaneOrientation::VERTICAL:
            newProps.height  = paneProps.height / 2;
            newProps.width   = paneProps.width;
            newProps.xOffset = 0;
            newProps.yOffset = paneProps.height / 2;
            break;
    }

    pane->paneProps = newProps;
}

void SplitPane::setSecondPane(IPane* pane) {
    secondPane = pane;

    PaneProps newProps = PaneProps();

    switch (orientation) {
        case PaneOrientation::HORIZONTAL:
            newProps.height  = paneProps.height;
            newProps.width   = paneProps.width / 2;
            newProps.xOffset = 0;
            newProps.yOffset = 0;
            break;
        case PaneOrientation::VERTICAL:
            newProps.height  = paneProps.height / 2;
            newProps.width   = paneProps.width;
            newProps.xOffset = 0;
            newProps.yOffset = 0;
            break;
    }

    pane->paneProps = newProps;
}
