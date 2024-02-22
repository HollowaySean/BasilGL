#ifndef SRC_WINDOW_IPANE_HPP_
#define SRC_WINDOW_IPANE_HPP_

namespace basil {

/** @brief Struct containing pane size & offset. */
struct PaneProps {
    int width, height;
    int xOffset, yOffset;
};

/** @brief Interface consumable by WindowView, describing
 *  a single section of the top-level window.
 */
class IPane {
 public:
    /** @param paneProps Struct containing pane size & offset. */
    explicit IPane(PaneProps paneProps): paneProps(paneProps) {}

    /** @brief Render contents of pane. */
    virtual void const draw() = 0;

    /** @brief Callback for frame resize. */
    virtual void onResize(int newWidth, int newHeight) {
        paneProps.width = newWidth;
        paneProps.height = newHeight;
    }

    PaneProps paneProps;
};

}  // namespace basil

#endif  // SRC_WINDOW_IPANE_HPP_
