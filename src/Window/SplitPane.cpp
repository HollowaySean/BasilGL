#include "SplitPane.hpp"

namespace basil {

SplitPane::SplitPane(): IPane(PaneProps()), orientation(HORIZONTAL) {
    resizeToPercentage(50.f);
    updateSize();
}

SplitPane::SplitPane(PaneProps paneProps, PaneOrientation orientation):
        IPane(paneProps), orientation(orientation) {
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
    IPane::onResize(newWidth, newHeight);

    resizeToPercentage(percentageExtent);
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

void SplitPane::setFirstPane(std::shared_ptr<IPane> pane) {
    if (pane == secondPane) {
        logger.log("First pane has same address as second. Pane not set.",
            LogLevel::WARN);
    } else {
        firstPane = pane;
    }

    updateSize();
}

void SplitPane::setSecondPane(std::shared_ptr<IPane> pane) {
    if (pane == firstPane) {
        logger.log("Second pane has same address as first. Pane not set.",
            LogLevel::WARN);
    } else {
        secondPane = pane;
    }

    updateSize();
}

float SplitPane::getFirstPaneSizeAsPercentage() {
    return percentageExtent;
}

float SplitPane::getSecondPaneSizeAsPercentage() {
    return 100. - getFirstPaneSizeAsPercentage();
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
    if (extent < 0) {
        extent = 0;
    } else if (extent > 100) {
        extent = 100;
    }

    percentageExtent = extent;

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

SplitPane::Builder&
SplitPane::Builder::withFirstPane(std::shared_ptr<IPane> firstPane) {
    impl->setFirstPane(firstPane);
    return (*this);
}

SplitPane::Builder&
SplitPane::Builder::withSecondPane(std::shared_ptr<IPane> secondPane) {
    impl->setSecondPane(secondPane);
    return (*this);
}

SplitPane::Builder&
SplitPane::Builder::withOrientation(PaneOrientation orientation) {
    impl->setOrientation(orientation);
    return (*this);
}

SplitPane::Builder&
SplitPane::Builder::withGapWidth(int gapWidth) {
    impl->setGapWidth(gapWidth);
    return (*this);
}

SplitPane::Builder&
SplitPane::Builder::withPaneExtentInPixels(int extent) {
    impl->resizeToPixelValue(extent);
    return (*this);
}

SplitPane::Builder&
SplitPane::Builder::withPaneExtentInPercent(float extent) {
    impl->resizeToPercentage(extent);
    return (*this);
}

}  // namespace basil
