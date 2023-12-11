#ifndef SRC_WINDOW_IPANE_HPP_
#define SRC_WINDOW_IPANE_HPP_

struct PaneProps {
    int width, height;
    int xOffset, yOffset;
};

/** @brief Interface consumable by WindowView, describing
 *  a single section of the top-level window.
 */
class IPane {
 public:
    explicit IPane(PaneProps paneProps): paneProps(paneProps) {}
    virtual void const draw() = 0;
    // virtual void destroy() = 0;
    // virtual void onResize(int newWidth, int newHeight) {}
    // virtual void onClick(int clickX, int clickY) {}
    // virtual void onKeyPress() {}

    PaneProps paneProps;
};

#endif  // SRC_WINDOW_IPANE_HPP_
