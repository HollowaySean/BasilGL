#ifndef SRC_APPLICATION_BASILAPP_HPP_
#define SRC_APPLICATION_BASILAPP_HPP_

#include <memory>

#include <Basil/Context.hpp>
#include <Basil/Frame.hpp>
#include <Basil/Window.hpp>

namespace basil {

// TODO(sholloway): Read this https://www.slideshare.net/DimitriosPlatis/builder-pattern-in-cpdf

class BasilApp : private BasilContextDependency {
 public:
    /** @brief Start application. */
    void run();

    /** @brief Builder pattern to attach core components. */
    class Builder {
     public:
        BasilApp build();

        Builder& withFrameRateCap(int maxFrameRate);
        Builder& withMetricsReporter(int bufferSize = 20);
        Builder& withShaderPane(std::filesystem::path shaderPath);

     private:
        friend BasilApp;

        // Process related
        std::optional<int> frameRateCap;
        // TODO(sholloway): Figure out how to do builders
        bool shouldIncludeMetrics = false;
        int bufferSize = 20;

        // Window related
        std::shared_ptr<IPane> topPane;
    };

    BasilApp();

 private:
    explicit BasilApp(Builder builder);

    std::shared_ptr<IPane> topPane;
    std::shared_ptr<WindowView> windowView;
    std::shared_ptr<FrameController> frameController;
};

}  // namespace basil

#endif  // SRC_APPLICATION_BASILAPP_HPP_
