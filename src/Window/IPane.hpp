#pragma once

#include <Basil/PubSub.hpp>

namespace basil {

/** @brief Struct containing pane size & offset. */
struct PaneProps {
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

}   // namespace basil
