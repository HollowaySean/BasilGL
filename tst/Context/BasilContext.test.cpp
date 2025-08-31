#include <catch.hpp>

#include "Context/BasilContext.hpp"
#include "OpenGL/GLTestUtils.hpp"

using basil::BasilContext;
using basil::Logger;
using basil::LogLevel;

// TEST_CASE("Context_BasilContext_setGLFW[...]Callback") { BASIL_LOCK_TEST
//     bool hasBeenCalled = false;
//     auto callback =
//         [&](int...){ hasBeenCalled = true; };

//     SECTION("setGLFWFramebufferSizeCallback") {
//         unsigned int ID =
//             BasilContext::setGLFWFramebufferSizeCallback(callback);
//         CHECK(BasilContext::framebufferCallbacks.contains(ID));

//         CHECK_FALSE(hasBeenCalled);
//         BasilContext::onFrameBufferResize(nullptr, 0, 0);
//         CHECK(hasBeenCalled);

//         BasilContext::removeGLFWFramebufferSizeCallback(ID);
//         CHECK_FALSE(BasilContext::framebufferCallbacks.contains(ID));
//     }

//     SECTION("setGLFWMouseButtonCallback") {
//         unsigned int ID =
//             BasilContext::setGLFWMouseButtonCallback(callback);
//         CHECK(BasilContext::mouseButtonCallbacks.contains(ID));

//         CHECK_FALSE(hasBeenCalled);
//         BasilContext::onMouseButton(nullptr, 0, 0, 0);
//         CHECK(hasBeenCalled);

//         BasilContext::removeGLFWMouseButtonCallback(ID);
//         CHECK_FALSE(BasilContext::mouseButtonCallbacks.contains(ID));
//     }

//     SECTION("setGLFWKeyCallback") {
//         unsigned int ID =
//             BasilContext::setGLFWKeyCallback(callback);
//         CHECK(BasilContext::keyCallbacks.contains(ID));

//         BasilContext::setGLFWKeyCallback(callback);
//         CHECK_FALSE(hasBeenCalled);
//         BasilContext::onKeyAction(nullptr, 0, 0, 0, 0);
//         CHECK(hasBeenCalled);

//         BasilContext::removeGLFWKeyCallback(ID);
//         CHECK_FALSE(BasilContext::keyCallbacks.contains(ID));
//     }

//     SECTION("setGLFWCursorEnterCallback") {
//         unsigned int ID =
//             BasilContext::setGLFWCursorEnterCallback(callback);
//         CHECK(BasilContext::cursorEnterCallbacks.contains(ID));

//         BasilContext::setGLFWCursorEnterCallback(callback);
//         CHECK_FALSE(hasBeenCalled);
//         BasilContext::onCursorEnter(nullptr, 0);
//         CHECK(hasBeenCalled);

//         BasilContext::removeGLFWCursorEnterCallback(ID);
//         CHECK_FALSE(BasilContext::cursorEnterCallbacks.contains(ID));
//     }
// }

// TEST_CASE("Context_BasilContext_logGLFWError") { BASIL_LOCK_TEST
//     Logger& logger = Logger::get();

//     SECTION("Logs info on success") {
//         BasilContext::logGLFWError(1);

//         CHECK(logger.getLastOutput() ==
//             "GLFW context initialized successfully.");
//         CHECK(logger.getLastLevel() ==
//             LogLevel::INFO);
//     }

//     SECTION("Logs error on failure") {
//         BasilContext::logGLFWError(0);

//         CHECK(logger.getLastLevel() == LogLevel::ERROR);
//     }
// }

// TEST_CASE("Context_BasilContext_logGLFWWindowError") { BASIL_LOCK_TEST
//     Logger& logger = Logger::get();

//     SECTION("Logs info on success") {
//         GLFWwindow* window = BasilContext::getGLFWWindow();
//         BasilContext::logGLFWWindowError(window);

//         CHECK(logger.getLastOutput() ==
//             "GLFW window created successfully.");
//         CHECK(logger.getLastLevel() ==
//             LogLevel::INFO);
//     }

//     SECTION("Logs error and terminates on failure") {
//         BasilContext::initialize();
//         CHECK(BasilContext::isInitialized());

//         BasilContext::logGLFWWindowError(nullptr);

//         CHECK(logger.getLastOutput() == "GLFW failed to create window.");
//         CHECK(logger.getLastLevel() == LogLevel::ERROR);
//         CHECK_FALSE(BasilContext::isInitialized());

//         // Re-initialize to prevent issues
//         BasilContext::initialize();
//     }
// }

// TEST_CASE("Context_BasilContext_logGLEWError") { BASIL_LOCK_TEST
//     Logger& logger = Logger::get();

//     SECTION("Logs info on success") {
//         BasilContext::logGLEWError(GLEW_OK);

//         CHECK(logger.getLastOutput() ==
//             "GLEW context initialized successfully.");
//         CHECK(logger.getLastLevel() ==
//             LogLevel::INFO);
//     }

//     SECTION("Logs error on failure") {
//         BasilContext::logGLEWError(!GLEW_OK);

//         CHECK(logger.getLastLevel() == LogLevel::ERROR);
//     }
// }
