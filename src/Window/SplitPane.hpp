#ifndef SRC_WINDOW_SPLITPANE_HPP_
#define SRC_WINDOW_SPLITPANE_HPP_

#include "IPane.hpp"

/** @brief Enum to set SplitPane orientation. */
enum PaneOrientation {
    HORIZONTAL, VERTICAL
};

/** @class SplitPane
 *  @brief IPane implementation which displays two panes inside.
*/
class SplitPane : public IPane {
 public:
    SplitPane(const PaneProps &paneProps,
        PaneOrientation orientation = PaneOrientation::HORIZONTAL):
            IPane(paneProps),
            firstPane(nullptr),
            secondPane(nullptr),
            orientation(orientation) {}

    /** @brief Calls draw on contained panes. */
    void const draw();

    /** @param pane Sets upper/left pane. */
    void setFirstPane(IPane* pane);

    /** @param pane Sets lower/right pane. */
    void setSecondPane(IPane* pane);

 private:
    PaneOrientation orientation;
    IPane* firstPane;
    IPane* secondPane;
};

#endif  // SRC_WINDOW_SPLITPANE_HPP_
