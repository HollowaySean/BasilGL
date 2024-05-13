#ifndef SRC_APP_IBASILWIDGET_HPP_
#define SRC_APP_IBASILWIDGET_HPP_

#include <memory>

#include <Basil/Process.hpp>
#include <Basil/PubSub.hpp>

namespace basil {

/** @brief Interface which encapsulates Model & Controller components for BasilApp.
 *  BasilApp will autowire IBasilWidgets to ProcessController and PubSub, if added
 *  via addWidget or added to builder with withWidget method.
 */
class IBasilWidget : public IProcess,
                     public IDataPublisher,
                     public IDataSubscriber {
 protected:
    IBasilWidget() = default;
    IBasilWidget(ProcessOrdinal ordinal, ProcessPrivilege privilege)
        : ordinal(ordinal), privilege(privilege) {}

    friend class BasilApp;

    ProcessOrdinal ordinal = ProcessOrdinal::MAIN;
    ProcessPrivilege privilege = ProcessPrivilege::NONE;
};

}  // namespace basil

#endif  // SRC_APP_IBASILWIDGET_HPP_
