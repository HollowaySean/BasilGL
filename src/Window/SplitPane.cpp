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

void SplitPane::onResize(int newWidth, int newHeight) {
    float currentPercentage;
    switch (orientation) {
        case HORIZONTAL:
            currentPercentage = 100. *
                static_cast<float>(firstPaneExtent) /
                static_cast<float>(paneProps.width);
            break;
        case VERTICAL:
            currentPercentage = 100. *
                static_cast<float>(firstPaneExtent) /
                static_cast<float>(paneProps.height);
            break;
    }

    IPane::onResize(newWidth, newHeight);

    resizeToPercentage(currentPercentage);
}

void SplitPane::updateSize() {
    if (firstPane) {
        switch (orientation) {
            case HORIZONTAL:
                firstPane->paneProps.width = firstPaneExtent;
                firstPane->paneProps.height = this->paneProps.height;
                break;

            case VERTICAL:
                firstPane->paneProps.width = this->paneProps.width;
                firstPane->paneProps.height = firstPaneExtent;
                break;
        }

        firstPane->paneProps.xOffset = this->paneProps.xOffset;
        firstPane->paneProps.yOffset = this->paneProps.yOffset;
    }

    if (secondPane) {
        switch (orientation) {
            case HORIZONTAL:
                secondPane->paneProps.width =
                    secondPaneExtent;
                secondPane->paneProps.height =
                    this->paneProps.height;
                secondPane->paneProps.xOffset =
                    this->paneProps.xOffset + firstPaneExtent + gapWidth;
                secondPane->paneProps.yOffset =
                    this->paneProps.yOffset;
                break;

            case VERTICAL:
                secondPane->paneProps.width =
                    this->paneProps.width;
                secondPane->paneProps.height =
                    secondPaneExtent;
                secondPane->paneProps.xOffset =
                    this->paneProps.xOffset;
                secondPane->paneProps.yOffset =
                    this->paneProps.yOffset + firstPaneExtent + gapWidth;
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
