#ifndef SRC_WINDOW_IPANE_HPP_
#define SRC_WINDOW_IPANE_HPP_

namespace basil {

/** @brief Struct containing pane size & offset. */
// TODO(sholloway): Remove hard coded values
struct PaneProps {
    int width = 100;
    int height = 100;
    int xOffset = 0;
    int yOffset = 0;
};

/** @brief Interface consumable by WindowView, describing
 *  a single section of the top-level window.
 */
class IPane {
 public:
    /** @param paneProps Struct containing pane size & offset. */
    explicit IPane(PaneProps paneProps): paneProps(paneProps) {}

    /** @brief Returns IPane with default paneProps. */
    IPane(): paneProps(PaneProps()) {}

    /** @brief Render contents of pane. */
    virtual void const draw() = 0;

    /** @brief Update PaneProps. */
    virtual void setPaneProps(PaneProps paneProps) {
        this->paneProps = paneProps;
        onResize(paneProps.width, paneProps.height);
    }

    /** @brief Callback for frame resize. */
    virtual void onResize(int newWidth, int newHeight) {
        paneProps.width = newWidth;
        paneProps.height = newHeight;
    }

    PaneProps paneProps;
};

}  // namespace basil

#endif  // SRC_WINDOW_IPANE_HPP_
