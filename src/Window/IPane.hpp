#pragma once

#include <Basil/Packages/PubSub.hpp>

namespace basil {

/** @brief Struct containing pane size & offset. */
struct ViewArea {
    int width = 0;
    int height = 0;
    int xOffset = 0;
    int yOffset = 0;
};

/** @brief Interface consumable by WindowView, describing
 *  a single section of the top-level window.
 */
class IPane : public IDataPassThrough {
 public:
    /** @param viewArea Struct containing pane size & offset. */
    explicit IPane(ViewArea viewArea): viewArea(viewArea) {}

    /** @brief Returns IPane with default viewArea. */
    IPane(): viewArea(ViewArea()) {}

    /** @brief Render contents of pane. */
    virtual void const draw() = 0;

    /** @brief Update ViewArea. */
    virtual void setPaneProps(ViewArea viewArea) {
        this->viewArea = viewArea;
        onResize(viewArea.width, viewArea.height);
    }

    /** @brief Callback for frame resize. */
    virtual void onResize(int newWidth, int newHeight) {
        viewArea.width = newWidth;
        viewArea.height = newHeight;
    }

    ViewArea viewArea;
};

}   // namespace basil
