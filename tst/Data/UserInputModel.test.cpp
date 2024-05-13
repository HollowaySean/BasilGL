#include <GLFW/glfw3.h>

#include <catch.hpp>

#include "Data/UserInputModel.hpp"

using basil::UserInputModel;

TEST_CASE("Data_UserInputModel_setKeyState") {
    auto model = UserInputModel();

    SECTION("Saves key state data to model") {
        uint keyCode = GLFW_KEY_DELETE;
        int state = GLFW_PRESS;
        int mods = GLFW_MOD_CONTROL | GLFW_MOD_ALT;

        model.setKeyState(keyCode, state, mods);

        CHECK(model.getIsKeyPressed(keyCode));
        CHECK(model.getKeyState(keyCode).modifiers == mods);
        CHECK(model.getKeyState(keyCode).pressState == state);
    }
}

TEST_CASE("Data_UserInputModel_setMouseButtonState") {
    auto model = UserInputModel();

    SECTION("Saves button state data to model") {
        uint buttonCode = GLFW_MOUSE_BUTTON_LEFT;
        int state = GLFW_PRESS;
        int mods = GLFW_MOD_CONTROL | GLFW_MOD_ALT;

        model.setMouseButtonState(buttonCode, state, mods);

        CHECK(model.getIsMouseButtonPressed(buttonCode));
        CHECK(model.getMouseButtonState(buttonCode).modifiers == mods);
        CHECK(model.getMouseButtonState(buttonCode).pressState == state);
    }
}

TEST_CASE("Data_UserInputModel_setMousePosition") {
    auto model = UserInputModel();

    SECTION("Saves mouse position to model") {
        double xPos = 45.3;
        double yPos = 999.9;

        model.setMousePosition(xPos, yPos);

        CHECK(model.getMousePosition().xPosition == xPos);
        CHECK(model.getMousePosition().yPosition == yPos);
    }
}

TEST_CASE("Data_UserInputModel_setIsMouseInWindow") {
    auto model = UserInputModel();

    SECTION("Saves mouse position to model") {
        model.setIsMouseInWindow(true);

        CHECK(model.getMousePosition().isInWindow);
    }
}
