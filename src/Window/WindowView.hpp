#pragma once

#include <memory>
#include <optional>
#include <string>

#include <Basil/Packages/Builder.hpp>
#include <Basil/Packages/Context.hpp>
#include <Basil/Packages/PubSub.hpp>

#include "App/IBasilWidget.hpp"

#include "IPane.hpp"

#include "Definitions.hpp"

namespace basil {

/**
 * @brief Struct holding basic properties of window.
*/
struct WindowProps {
 public:
    #ifndef TEST_BUILD
    inline static const int DEFAULT_WIDTH = BASIL_DEFAULT_WINDOW_WIDTH;
    inline static const int DEFAULT_HEIGHT = BASIL_DEFAULT_WINDOW_HEIGHT;
    #else
    inline static const int DEFAULT_WIDTH = BASIL_DEFAULT_WINDOW_WIDTH_TEST;
    inline static const int DEFAULT_HEIGHT = BASIL_DEFAULT_WINDOW_HEIGHT_TEST;
    #endif

    inline static const std::string DEFAULT_TITLE = BASIL_DEFAULT_WINDOW_TITLE;

    /** @brief Title to display in OS window banner.  */
    std::string title = DEFAULT_TITLE;

    /** @brief Width of window in screen coordinates. */
    int width = DEFAULT_WIDTH;

    /** @brief Height of window in screen coordinates. */
    int height = DEFAULT_HEIGHT;
};

/**
 * @brief Outer window containing all UI elements and providing simple
 * public facade.
 */
class WindowView :  public IBasilWidget,
                    public IBuildable<WindowView>,
                    private IBasilContextConsumer {
 public:
    /** @param windowProps Optional struct containing window options. */
    explicit WindowView(std::optional<WindowProps> windowProps = std::nullopt);

    /** @brief Makes window visible. */
    void onStart() override;

    /** @brief Main loop function for IProcess parent class. */
    void onLoop() override;

    /** @brief Passthrough override for PubSub. */
    void receiveData(const DataMessage& message) override;

    /** @brief Sets top-level pane for window. */
    void setTopPane(std::shared_ptr<IPane> newTopPane);

    /** @returns ViewArea object for top pane. */
    ViewArea getTopPaneProps();

    /** @brief Update window size & title. */
    void setWindowProps(const WindowProps& newWindowProps);

    /** @brief Update window size. */
    void setWindowSize(int width, int height);

    /** @brief Update window title. */
    void setWindowTitle(const std::string& title);

    /** @returns Window settings as struct. */
    WindowProps getWindowProps () { return windowProps; }

    /** Builder pattern to construct WindowView */
    class Builder : public IBuilder<WindowView> {
     public:
        /** @brief Set height and width of window. */
        Builder& withDimensions(int width, int height);

        /** @brief Set title of window. */
        Builder& withTitle(const std::string& title);

        /** @brief Set top IPane object for window. */
        Builder& withTopPane(std::shared_ptr<IPane> topPane);
    };

#ifndef TEST_BUILD

 private:
#endif
    WindowProps windowProps;

    GLFWwindow* glfwWindow;

    void draw();
    void closeWindow();
    void setCallbacks();
    void removeCallbacks();
    void onResize(int width, int height);

    unsigned int resizeCallbackID = -1;

    std::shared_ptr<IPane> topPane;

    Logger& logger = Logger::get();
};

}   // namespace basil
