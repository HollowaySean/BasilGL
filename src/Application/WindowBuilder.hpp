#ifndef SRC_APPLICATION_WINDOWBUILDER_HPP_
#define SRC_APPLICATION_WINDOWBUILDER_HPP_

#include <memory>
#include <utility>
#include <string>

#include "IBuilder.hpp"

#include <Basil/Window.hpp>

namespace basil {

class WindowBuilder : public IBuilder<WindowView> {
 public:
    WindowBuilder& withDimensions(int width, int height);
    WindowBuilder& withTitle(const std::string& title);
};

}  // namespace basil

#endif  // SRC_APPLICATION_WINDOWBUILDER_HPP_
