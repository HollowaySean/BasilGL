#include "SplitPane.hpp"

SplitPane::SplitPane(const PaneProps &paneProps, PaneOrientation orientation):
        IPane(paneProps),
        firstPane(nullptr),
        secondPane(nullptr),
        orientation(orientation),
        gapWidth(0) {
    // Default to even 50% split
    resizeToPercentage(50.f);
    updateSize();
}

void const SplitPane::draw() {
    if (firstPane) {
        firstPane->draw();
    }
    if (secondPane) {
        secondPane->draw();
    }
}

void SplitPane::updateSize() {
    if (firstPane) {
        switch (orientation) {
            case HORIZONTAL:
                firstPane->paneProps.width = firstPaneExtent;
                firstPane->paneProps.height = this->paneProps.height;
                firstPane->paneProps.xOffset = 0;
                firstPane->paneProps.yOffset = 0;

                break;

            case VERTICAL:
                firstPane->paneProps.width = this->paneProps.width;
                firstPane->paneProps.height = firstPaneExtent;
                firstPane->paneProps.xOffset = 0;
                firstPane->paneProps.yOffset = 0;

                break;
        }
    }

    if (secondPane) {
        switch (orientation) {
            case HORIZONTAL:
                secondPane->paneProps.width = secondPaneExtent;
                secondPane->paneProps.height = this->paneProps.height;
                secondPane->paneProps.xOffset = firstPaneExtent + gapWidth;
                secondPane->paneProps.yOffset = 0;

                break;

            case VERTICAL:
                secondPane->paneProps.width = this->paneProps.width;
                secondPane->paneProps.height = secondPaneExtent;
                secondPane->paneProps.xOffset = 0;
                secondPane->paneProps.yOffset = firstPaneExtent + gapWidth;

                break;
        }
    }
}

void SplitPane::setFirstPane(IPane* pane) {
    firstPane = pane;
    updateSize();
}

void SplitPane::setSecondPane(IPane* pane) {
    secondPane = pane;
    updateSize();
}

void SplitPane::resizeToPixelValue(int extent) {
    firstPaneExtent = extent;

    switch (orientation) {
        case HORIZONTAL:
            secondPaneExtent = paneProps.width - gapWidth - firstPaneExtent;
            break;
        case VERTICAL:
            secondPaneExtent = paneProps.height - gapWidth - firstPaneExtent;
            break;
    }

    updateSize();
}

void SplitPane::resizeToPercentage(float extent) {
    if (extent < 0 || extent > 100) {
        return;
    }

    int extentInPixels;
    switch (orientation) {
        case HORIZONTAL:
            extentInPixels = paneProps.width * extent / 100.f;
            break;
        case VERTICAL:
            extentInPixels = paneProps.height * extent / 100.f;
            break;
    }

    resizeToPixelValue(extentInPixels);
}

void SplitPane::setGapWidth(int extent) {
    this->gapWidth = extent;

    firstPaneExtent -= gapWidth / 2;

    switch (orientation) {
        case HORIZONTAL:
            secondPaneExtent = paneProps.width - gapWidth - firstPaneExtent;
            break;
        case VERTICAL:
            secondPaneExtent = paneProps.height - gapWidth - firstPaneExtent;
            break;
    }

    updateSize();
}

void SplitPane::setOrientation(PaneOrientation orientation) {
    this->orientation = orientation;

    updateSize();
}
