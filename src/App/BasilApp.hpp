#pragma once

#include <memory>
#include <vector>

#include "IBasilWidget.hpp"

#include <Basil/Packages/Builder.hpp>
#include <Basil/Packages/Logging.hpp>
#include <Basil/Packages/Process.hpp>
#include <Basil/Packages/PubSub.hpp>

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

    /** @brief Registers widget with app. */
    void addWidget(std::shared_ptr<IBasilWidget> widget);

    /** @brief Sends message to app's PubSub publisher. */
    void sendMessage(DataMessage message);

    /** @brief Builder pattern for BasilApp. */
    class Builder : public IBuilder<BasilApp> {
     public:
        /** @brief Sets ProcessController */
        Builder& withController(std::shared_ptr<ProcessController> controller);

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
    void autoWireWidget(std::shared_ptr<IBasilWidget> widget);
    void autoWireWidgetProcess(std::shared_ptr<IBasilWidget> widget);
    void autoWireWidgetPublisher(std::shared_ptr<IBasilWidget> widget);

    std::vector<std::shared_ptr<IBasilWidget>> widgets;

    std::shared_ptr<IDataPassThrough> publisher
        = std::make_shared<IDataPassThrough>();

    std::shared_ptr<ProcessController> processController;
};

}   // namespace basil
