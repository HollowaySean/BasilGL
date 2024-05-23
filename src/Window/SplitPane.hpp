#pragma once

#include <memory>

#include <Basil/Packages/Builder.hpp>
#include <Basil/Packages/Logging.hpp>

#include "IPane.hpp"

namespace basil {

/** @brief IPane implementation which displays two panes inside. */
class SplitPane :   public IPane,
                    public IBuildable<SplitPane> {
 public:
    /** @brief Enum to set SplitPane orientation. */
    enum class Orientation {
        HORIZONTAL, VERTICAL
    };

    /** @brief Enum to set SplitPane invariant. */
    enum class Invariant {
        PIXELS, PERCENTAGE, ASPECT
    };

    /** @brief Enum to set pane invariant. */
    enum class FixedPane {
        FIRST, SECOND
    };

    /** @brief Initialize SplitPane with optional orientation setting. */
    explicit SplitPane(
        SplitPane::Orientation orientation
            = SplitPane::Orientation::HORIZONTAL);

    /** @brief Initialize SplitPane with PaneProps and optional orientation. */
    SplitPane(PaneProps paneProps,
        SplitPane::Orientation orientation
            = SplitPane::Orientation::HORIZONTAL);

    /** @brief Calls draw on contained panes. */
    void const draw() override;

    /** @brief Resizes child panes upon resize. */
    void onResize(int newWidth, int newHeight) override;

    /** @param pane Sets upper/left pane. */
    void setFirstPane(std::shared_ptr<IPane> pane);

    /** @param pane Sets lower/right pane. */
    void setSecondPane(std::shared_ptr<IPane> pane);

    /** @param fixedPane Sets which pane remains invariant. */
    void setFixedPane(SplitPane::FixedPane fixedPane);

    /** @param extent Size of fixed pane, in pixels. */
    void setPaneSizeInPixels(int extent,
        std::optional<SplitPane::FixedPane> paneToSet = std::nullopt);

    /** @param extent Size of fixed pane, as a percentage. */
    void setPaneSizeAsPercentage(float extent,
        std::optional<SplitPane::FixedPane> paneToSet = std::nullopt);

    /** @param aspectRatio Aspect ratio of fixed pane, as ratio. */
    void setPaneAspectRatio(float aspectRatio,
        std::optional<SplitPane::FixedPane> paneToSet = std::nullopt);

    /** @param extent Size of gap between panes, in pixels. */
    void setGapWidth(int extent);

    /** @param orientation Set orientation of split. */
    void setOrientation(SplitPane::Orientation orientation);

    /** @returns Size of first pane, in pixels. */
    int getFirstPaneSizeInPixels() { return firstPaneExtent; }

    /** @returns Size of second pane, in pixels. */
    int getSecondPaneSizeInPixels() { return secondPaneExtent; }

    /** @returns Size of first pane, as a percentage. */
    float getFirstPaneSizeAsPercentage();

    /** @returns Size of first pane, as a percentage. */
    float getSecondPaneSizeAsPercentage();

    /** @returns Aspect ratio of first pane. */
    float getFirstPaneAspectRatio();

    /** @returns Aspect ratio of second pane. */
    float getSecondPaneAspectRatio();

    /** @returns Size of gap between panels, in pixels. */
    int getGapWidth() { return settings.gapWidth; }

    /** @returns Orientation of split. */
    SplitPane::Orientation getOrientation() { return settings.orientation; }

    /** @brief Builder pattern for SplitPane. */
    class Builder : public IBuilder<SplitPane> {
     public:
        /** @brief Set paneProps struct. */
        Builder& withPaneProps(PaneProps paneProps);

        /** @brief Sets top/left pane. */
        Builder& withFirstPane(std::shared_ptr<IPane> firstPane);

        /** @brief Sets bottom/right pane. */
        Builder& withSecondPane(std::shared_ptr<IPane> secondPane);

        /** @brief Sets vertical vs. horizontal split. */
        Builder& withOrientation(SplitPane::Orientation orientation);

        /** @brief Sets which pane is fixed at a given size/aspect ratio. */
        Builder& withFixedPane(SplitPane::FixedPane fixedPane);

        /** @brief Sets gap between panes, in pixels. */
        Builder& withGapWidth(int gapWidth);

        /** @brief Sets size of fixed pane, in pixels. */
        Builder& withPaneExtentInPixels(int extent);

        /** @brief Sets size of fixed pane, as a percentage. */
        Builder& withPaneExtentInPercent(float extent);

        /** @brief Sets size of fixed pane, as a percentage. */
        Builder& withPaneAspectRatio(float aspectRatio);
    };

#ifndef TEST_BUILD

 private:
#endif
    struct SplitPaneSettings {
        SplitPane::Invariant invariant = SplitPane::Invariant::PERCENTAGE;
        SplitPane::Orientation orientation = SplitPane::Orientation::HORIZONTAL;
        SplitPane::FixedPane fixedPane = SplitPane::FixedPane::FIRST;
        unsigned int gapWidth = 0;
        unsigned int fixedPixels = 0;
        float fixedPercentage = 50.f;
        float fixedAspect = 1.f;
    };

    void updateSize();
    void updateSizeByPixels();
    void updateSizeByPercentage();
    void updateSizeByAspect();

    void updateExtents(
        unsigned int fixedPaneExtent,
        unsigned int nonFixedPaneExtent);

    void setPanePropsFromSizes();

    unsigned int getOnAxisExtent();
    unsigned int getOffAxisExtent();
    float getTotalAspect();

    unsigned int firstPaneExtent;
    unsigned int secondPaneExtent;

    Logger& logger = Logger::get();

    std::shared_ptr<IPane> firstPane = nullptr;
    std::shared_ptr<IPane> secondPane = nullptr;

    SplitPaneSettings settings = SplitPaneSettings();
};

}   // namespace basil
