#pragma once

#include <memory>
#include <string>

#include <Basil/Process.hpp>
#include <Basil/PubSub.hpp>

namespace basil {

/** @brief Enum for setting PubSub preferences on widget. */
enum class WidgetPubSubPrefs {
    NONE, PUBLISH_ONLY, SUBSCRIBE_ONLY, ALL
};

/** @brief Struct holding widget preferences. */
struct WidgetPrefs {
    std::string defaultName = "";
    ProcessOrdinal ordinal = ProcessOrdinal::MAIN;
    ProcessPrivilege privilege = ProcessPrivilege::NONE;
    WidgetPubSubPrefs pubSubPrefs = WidgetPubSubPrefs::NONE;
};

/** @brief Interface which encapsulates Model & Controller components for BasilApp.
 *  BasilApp will autowire IBasilWidgets to ProcessController and PubSub, if added
 *  via addWidget or added to builder with withWidget method.
 */
class IBasilWidget : public IProcess,
                     public IDataPublisher,
                     public IDataSubscriber {
#ifdef TEST_BUILD
 public:
#else
 protected:
#endif

    IBasilWidget() = default;

    explicit IBasilWidget(const std::string& defaultName) {
        if (!defaultName.empty()) {
            setProcessName(defaultName);
        }
    }

    explicit IBasilWidget(WidgetPrefs prefs)
            : ordinal(prefs.ordinal),
              privilege(prefs.privilege),
              pubSubPrefs(prefs.pubSubPrefs) {
        if (!prefs.defaultName.empty()) {
            setProcessName(prefs.defaultName);
        }
    }

    friend class BasilApp;

    bool shouldSubscribeToApp() {
        return pubSubPrefs == WidgetPubSubPrefs::SUBSCRIBE_ONLY
            || pubSubPrefs == WidgetPubSubPrefs::ALL;
    }

    bool shouldPublishToApp() {
        return pubSubPrefs == WidgetPubSubPrefs::PUBLISH_ONLY
            || pubSubPrefs == WidgetPubSubPrefs::ALL;
    }

    ProcessOrdinal ordinal = ProcessOrdinal::MAIN;
    ProcessPrivilege privilege = ProcessPrivilege::NONE;
    WidgetPubSubPrefs pubSubPrefs = WidgetPubSubPrefs::NONE;
};

}   // namespace basil
