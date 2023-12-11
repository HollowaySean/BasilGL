#ifndef SRC_WINDOW_SPLITPANE_HPP_
#define SRC_WINDOW_SPLITPANE_HPP_

#include "IPane.hpp"

enum PaneOrientation {
    HORIZONTAL, VERTICAL
};

class SplitPane : public IPane {
 public:
    SplitPane(const PaneProps &paneProps,
        PaneOrientation orientation = PaneOrientation::HORIZONTAL):
            IPane(paneProps),
            firstPane(nullptr),
            secondPane(nullptr),
            orientation(orientation) {}
    void const draw();

    void setFirstPane(IPane* pane);
    void setSecondPane(IPane* pane);

 private:
    PaneOrientation orientation;
    IPane* firstPane;
    IPane* secondPane;
};

#endif  // SRC_WINDOW_SPLITPANE_HPP_
