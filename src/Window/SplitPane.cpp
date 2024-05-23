#include "SplitPane.hpp"

namespace basil {

SplitPane::SplitPane(Orientation orientation) {
    settings.orientation = orientation;
    firstPaneExtent = 0;
    secondPaneExtent = 0;
}

SplitPane::SplitPane(PaneProps paneProps,
        Orientation orientation) : IPane(paneProps) {
    settings.orientation = orientation;
    firstPaneExtent = 0;
    secondPaneExtent = 0;
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

    updateSize();
}

void SplitPane::setFirstPane(std::shared_ptr<IPane> pane) {
    if (firstPane) {
        this->IDataPublisher::unsubscribe(firstPane);
    }

    if (pane == secondPane) {
        logger.log("First pane has same address as second. Pane not set.",
            LogLevel::WARN);
    } else {
        firstPane = pane;
    }

    updateSize();
    this->IDataPublisher::subscribe(pane);
}

void SplitPane::setSecondPane(std::shared_ptr<IPane> pane) {
    if (secondPane) {
        this->IDataPublisher::unsubscribe(secondPane);
    }

    if (pane == firstPane) {
        logger.log("Second pane has same address as first. Pane not set.",
            LogLevel::WARN);
    } else {
        secondPane = pane;
    }

    updateSize();
    this->IDataPublisher::subscribe(pane);
}

void SplitPane::setFixedPane(SplitPane::FixedPane fixedPane) {
    settings.fixedPane = fixedPane;
    updateSize();
}

void SplitPane::setPaneSizeInPixels(int extent,
        std::optional<SplitPane::FixedPane> paneToSet) {
    auto setPane = paneToSet.value_or(settings.fixedPane);

    settings.invariant = SplitPane::Invariant::PIXELS;
    settings.fixedPane = setPane;
    settings.fixedPixels = extent;

    updateSizeByPixels();
}

void SplitPane::setPaneSizeAsPercentage(float extent,
        std::optional<SplitPane::FixedPane> paneToSet) {
    auto setPane = paneToSet.value_or(settings.fixedPane);

    settings.invariant = SplitPane::Invariant::PERCENTAGE;
    settings.fixedPane = setPane;
    settings.fixedPercentage = extent;

    updateSizeByPercentage();
}

void SplitPane::setPaneAspectRatio(float aspectRatio,
        std::optional<SplitPane::FixedPane> paneToSet) {
    auto setPane = paneToSet.value_or(settings.fixedPane);

    settings.invariant = SplitPane::Invariant::ASPECT;
    settings.fixedPane = setPane;
    settings.fixedAspect = aspectRatio;

    updateSizeByAspect();
}

void SplitPane::setGapWidth(int extent) {
    settings.gapWidth = extent;

    updateSize();
}

void SplitPane::setOrientation(Orientation orientation) {
    settings.orientation = orientation;

    updateSize();
}

float SplitPane::getFirstPaneSizeAsPercentage() {
    return 100. * static_cast<float>(firstPaneExtent) / getOnAxisExtent();
}

float SplitPane::getSecondPaneSizeAsPercentage() {
    return 100. * static_cast<float>(secondPaneExtent) / getOnAxisExtent();
}

float SplitPane::getFirstPaneAspectRatio() {
    return settings.orientation == Orientation::HORIZONTAL
        ? static_cast<float>(firstPaneExtent) / paneProps.height
        : static_cast<float>(paneProps.width) / firstPaneExtent;
}

float SplitPane::getSecondPaneAspectRatio() {
    return settings.orientation == Orientation::HORIZONTAL
        ? static_cast<float>(secondPaneExtent) / paneProps.height
        : static_cast<float>(paneProps.width) / secondPaneExtent;
}

void SplitPane::updateSize() {
    switch (settings.invariant) {
        case SplitPane::Invariant::PIXELS:
            updateSizeByPixels();
            break;
        case SplitPane::Invariant::PERCENTAGE:
            updateSizeByPercentage();
            break;
        case SplitPane::Invariant::ASPECT:
            updateSizeByAspect();
            break;
    }
}

void SplitPane::updateSizeByPixels() {
    int totalExtent = getOnAxisExtent();
    int extent = settings.fixedPixels;

    extent = (extent < 0) ? 0 : extent;
    extent = (extent > totalExtent) ? totalExtent : extent;

    unsigned int fixedPaneExtent = extent;
    unsigned int nonFixedPaneExtent =
        getOnAxisExtent() - settings.gapWidth - fixedPaneExtent;

    updateExtents(fixedPaneExtent, nonFixedPaneExtent);
    setPanePropsFromSizes();
}

void SplitPane::updateSizeByPercentage() {
    float extent = settings.fixedPercentage;
    extent = (extent < 0)   ? 0     : extent;
    extent = (extent > 100) ? 100   : extent;

    unsigned int fixedPaneExtent =
        getOnAxisExtent() * extent / 100.f;
    unsigned int nonFixedPaneExtent =
        getOnAxisExtent() - settings.gapWidth - fixedPaneExtent;

    updateExtents(fixedPaneExtent, nonFixedPaneExtent);
    setPanePropsFromSizes();
}

void SplitPane::updateSizeByAspect() {
    float aspectRatio = settings.fixedAspect;
    aspectRatio = (aspectRatio < 0) ? 0 : aspectRatio;

    float totalAspectRatio = getTotalAspect();
    switch (settings.orientation) {
        case Orientation::HORIZONTAL:
            aspectRatio = (aspectRatio > totalAspectRatio)
                ? totalAspectRatio : aspectRatio;
            break;
        case Orientation::VERTICAL:
            aspectRatio = (aspectRatio < totalAspectRatio)
                ? totalAspectRatio : aspectRatio;
            break;
    }

    unsigned int fixedPaneExtent =
        (settings.orientation == Orientation::HORIZONTAL)
            ? getOffAxisExtent() * aspectRatio
            : getOffAxisExtent() / aspectRatio;
    unsigned int nonFixedPaneExtent =
        getOnAxisExtent() - settings.gapWidth - fixedPaneExtent;

    updateExtents(fixedPaneExtent, nonFixedPaneExtent);
    setPanePropsFromSizes();
}

void SplitPane::setPanePropsFromSizes() {
    if (firstPane) {
        switch (settings.orientation) {
            case Orientation::HORIZONTAL:
                firstPane->paneProps.width = firstPaneExtent;
                firstPane->paneProps.height = this->paneProps.height;
                break;

            case Orientation::VERTICAL:
                firstPane->paneProps.width = this->paneProps.width;
                firstPane->paneProps.height = firstPaneExtent;
                break;
        }

        firstPane->paneProps.xOffset = this->paneProps.xOffset;
        firstPane->paneProps.yOffset = this->paneProps.yOffset;

        firstPane->onResize(
            firstPane->paneProps.width, firstPane->paneProps.height);
    }

    if (secondPane) {
        switch (settings.orientation) {
            case Orientation::HORIZONTAL:
                secondPane->paneProps.width =
                    secondPaneExtent;
                secondPane->paneProps.height =
                    this->paneProps.height;
                secondPane->paneProps.xOffset =
                    this->paneProps.xOffset
                        + firstPaneExtent + settings.gapWidth;
                secondPane->paneProps.yOffset =
                    this->paneProps.yOffset;
                break;

            case Orientation::VERTICAL:
                secondPane->paneProps.width =
                    this->paneProps.width;
                secondPane->paneProps.height =
                    secondPaneExtent;
                secondPane->paneProps.xOffset =
                    this->paneProps.xOffset;
                secondPane->paneProps.yOffset =
                    this->paneProps.yOffset
                        + firstPaneExtent + settings.gapWidth;
                break;
        }

        secondPane->onResize(
            secondPane->paneProps.width, secondPane->paneProps.height);
    }
}

void SplitPane::updateExtents(
        unsigned int fixedPaneExtent, unsigned int nonFixedPaneExtent) {
    switch (settings.fixedPane) {
        case SplitPane::FixedPane::FIRST:
            firstPaneExtent = fixedPaneExtent;
            secondPaneExtent = nonFixedPaneExtent;
            break;
        case SplitPane::FixedPane::SECOND:
            firstPaneExtent = nonFixedPaneExtent;
            secondPaneExtent = fixedPaneExtent;
            break;
    }
}

unsigned int SplitPane::getOnAxisExtent() {
    unsigned int extent =
        settings.orientation == Orientation::HORIZONTAL
            ? paneProps.width
            : paneProps.height;
    return extent;
}

unsigned int SplitPane::getOffAxisExtent() {
    unsigned int extent =
        settings.orientation == Orientation::HORIZONTAL
            ? paneProps.height
            : paneProps.width;
    return extent;
}

float SplitPane::getTotalAspect() {
    return static_cast<float>(paneProps.width)
         / static_cast<float>(paneProps.height);
}

SplitPane::Builder&
SplitPane::Builder::withPaneProps(PaneProps paneProps) {
    impl->setPaneProps(paneProps);
    return (*this);
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
SplitPane::Builder::withOrientation(Orientation orientation) {
    impl->setOrientation(orientation);
    return (*this);
}

SplitPane::Builder&
SplitPane::Builder::withFixedPane(SplitPane::FixedPane fixedPane) {
    impl->setFixedPane(fixedPane);
    return (*this);
}

SplitPane::Builder&
SplitPane::Builder::withGapWidth(int gapWidth) {
    impl->setGapWidth(gapWidth);
    return (*this);
}

SplitPane::Builder&
SplitPane::Builder::withPaneExtentInPixels(int extent) {
    impl->setPaneSizeInPixels(extent);
    return (*this);
}

SplitPane::Builder&
SplitPane::Builder::withPaneExtentInPercent(float extent) {
    impl->setPaneSizeAsPercentage(extent);
    return (*this);
}

SplitPane::Builder&
SplitPane::Builder::withPaneAspectRatio(float aspectRatio) {
    impl->setPaneAspectRatio(aspectRatio);
    return (*this);
}

}  // namespace basil
