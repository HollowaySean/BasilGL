#ifndef SRC_WINDOW_SPLITPANE_HPP_
#define SRC_WINDOW_SPLITPANE_HPP_

#include <memory>

#include <Basil/Builder.hpp>
#include <Basil/Logging.hpp>

#include "IPane.hpp"

namespace basil {

/** @brief Enum to set SplitPane orientation. */
enum PaneOrientation {
    HORIZONTAL, VERTICAL
};

/** @class SplitPane
 *  @brief IPane implementation which displays two panes inside.
*/
class SplitPane :   public IPane,
                    public IBuildable<SplitPane> {
 public:
    SplitPane();
    SplitPane(PaneProps paneProps,
        PaneOrientation orientation = PaneOrientation::HORIZONTAL);

    /** @brief Calls draw on contained panes. */
    void const draw() override;

    /** @brief Resizes child panes upon resize. */
    void onResize(int newWidth, int newHeight) override;

    /** @param pane Sets upper/left pane. */
    void setFirstPane(std::shared_ptr<IPane> pane);

    /** @param pane Sets lower/right pane. */
    void setSecondPane(std::shared_ptr<IPane> pane);

    /** @param extent Size of first pane, in pixels. */
    void resizeToPixelValue(int extent);

    /** @param extent Size of first pane, as a percentage. */
    void resizeToPercentage(float extent);

    /** @returns Size of first pane, in pixels. */
    int getFirstPaneSizeInPixels() { return firstPaneExtent; }

    /** @returns Size of second pane, in pixels. */
    int getSecondPaneSizeInPixels() { return secondPaneExtent; }

    /** @returns Size of first pane, as a percentage. */
    float getFirstPaneSizeAsPercentage();

    /** @returns Size of first pane, as a percentage. */
    float getSecondPaneSizeAsPercentage();

    /** @param extent Size of gap between panes, in pixels. */
    void setGapWidth(int extent);

    /** @returns Size of gap between panels, in pixels. */
    int getGapWidth() { return gapWidth; }

    /** @param orientation Set orientation of split. */
    void setOrientation(PaneOrientation orientation);

    /** @returns Orientation of split. */
    PaneOrientation getOrientation() { return orientation; }

    /** @brief Builder pattern for SplitPane. */
    class Builder : public IBuilder<SplitPane> {
     public:
        Builder& withFirstPane(std::shared_ptr<IPane> firstPane);
        Builder& withSecondPane(std::shared_ptr<IPane> secondPane);
        Builder& withOrientation(PaneOrientation orientation);
        Builder& withGapWidth(int gapWidth);
        Builder& withPaneExtentInPixels(int extent);
        Builder& withPaneExtentInPercent(float extent);
    };

 private:
    void updateSize();

    Logger& logger = Logger::get();

    PaneOrientation orientation = HORIZONTAL;
    std::shared_ptr<IPane> firstPane = nullptr;
    std::shared_ptr<IPane> secondPane = nullptr;

    int firstPaneExtent;
    int secondPaneExtent;
    float percentageExtent;
    int gapWidth = 0;
};

}  // namespace basil

#endif  // SRC_WINDOW_SPLITPANE_HPP_
