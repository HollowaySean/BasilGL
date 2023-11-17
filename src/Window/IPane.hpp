#ifndef SRC_WINDOW_IPANE_HPP_
#define SRC_WINDOW_IPANE_HPP_

/** @brief Interface consumable by WindowView, describing
 *  a single section of the top-level window.
 */
class IPane {
 public:
    IPane() = default;
    // virtual void setup(int width, int height) = 0;
    // virtual void draw() = 0;
    // virtual void destroy() = 0;
    // virtual void onResize(int newWidth, int newHeight) {}
    // virtual void onClick(int clickX, int clickY) {}
    // virtual void onKeyPress() {}

#ifndef TEST_BUILD

 protected:
#endif
    // int width, height;
    // WindowView *top;  // TODO(sholloway): Is this necessary?
    // IPane *parent;
};


#endif  // SRC_WINDOW_IPANE_HPP_
