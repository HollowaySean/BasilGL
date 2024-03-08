#ifndef SRC_APP_BASILAPP_HPP_
#define SRC_APP_BASILAPP_HPP_

#include <memory>

#include <Basil/Builder.hpp>
#include <Basil/Process.hpp>
#include <Basil/Window.hpp>

namespace basil {

// TODO(sholloway): Documentation
class BasilApp : public IBuildable<BasilApp> {
 public:
    void run();
    void stop();
    void kill();

    void setController(std::shared_ptr<ProcessController> controller) {
        this->processController = controller;
    }
    void setWindow(std::shared_ptr<WindowView> window) {
        this->windowView = window;
    }

    class Builder : public IBuilder<BasilApp> {
     public:
        Builder& withController(std::shared_ptr<ProcessController> controller);
        Builder& withWindow(std::shared_ptr<WindowView> window);

        std::unique_ptr<BasilApp> build() override;
    };

#ifndef TEST_BUILD

 private:
#endif
    std::shared_ptr<ProcessController> processController;
    std::shared_ptr<WindowView> windowView;
};

}  // namespace basil

#endif  // SRC_APP_BASILAPP_HPP_
