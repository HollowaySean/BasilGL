#ifndef SRC_APP_BASILAPP_HPP_
#define SRC_APP_BASILAPP_HPP_

#include <memory>
#include <vector>

#include "IBasilWidget.hpp"

#include <Basil/Builder.hpp>
#include <Basil/Process.hpp>
#include <Basil/Window.hpp>

namespace basil {

/** @brief Container and control interface for the major components
 *  of a Basil application.
 *
 *  @details Contains the following classes from the Basil library:
 *   * ProcessController - Runs the main loop and controls timing
 *   * WindowView - Renders operating system window via OpenGL
 **/
class BasilApp : public IBuildable<BasilApp> {
 public:
    /** @brief Start running application. */
    void run();

    /** @brief Gracefully stop processes. */
    void stop();

    /** @brief Stop processes immediately. */
    void kill();

    /** @brief Sets pointer to ProcessController for app. */
    void setController(std::shared_ptr<ProcessController> controller) {
        this->processController = controller;
    }

    /** @brief Sets pointer to WindowView for app. */
    void setWindow(std::shared_ptr<WindowView> window) {
        this->windowView = window;
    }

    void addWidget(std::shared_ptr<IBasilWidget> widget);

    /** @brief Builder pattern for BasilApp. */
    class Builder : public IBuilder<BasilApp> {
     public:
        /** @brief Sets ProcessController */
        Builder& withController(std::shared_ptr<ProcessController> controller);

        /** @brief Sets WindowView */
        Builder& withWindow(std::shared_ptr<WindowView> window);

        /** @brief Adds IBasilWidget to App */
        Builder& withWidget(std::shared_ptr<IBasilWidget> widget);

        /** @brief Override of base builder method */
        std::shared_ptr<BasilApp> build() override;
    };

#ifndef TEST_BUILD

 private:
#endif
    Logger& logger = Logger::get();
    void logControllerMissing();

    friend class Builder;
    void autoWire();
    void autoWireWindowProcess();
    void autoWireWidget(std::shared_ptr<IBasilWidget> widget);
    void autoWireWidgetProcess(std::shared_ptr<IBasilWidget> widget);
    void autoWireWidgetPublisher(std::shared_ptr<IBasilWidget> widget);

    std::vector<std::shared_ptr<IBasilWidget>> widgets;


    std::shared_ptr<IDataPassThrough> publisher
        = std::make_shared<IDataPassThrough>();

    std::shared_ptr<ProcessController> processController;
    std::shared_ptr<WindowView> windowView;
};

}  // namespace basil

#endif  // SRC_APP_BASILAPP_HPP_
