#include "WindowBuilder.hpp"

namespace basil {

WindowBuilder& WindowBuilder::withTitle(const std::string& title) {
    WindowProps currentProps = impl->getWindowProps();
    currentProps.title = title;
    impl->setWindowProps(currentProps);

    return (*this);
}

WindowBuilder& WindowBuilder::withDimensions(int width, int height) {
    WindowProps currentProps = impl->getWindowProps();
    currentProps.width = width;
    currentProps.height = height;
    impl->setWindowProps(currentProps);

    return (*this);
}

}  // namespace basil
