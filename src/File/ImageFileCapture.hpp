#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <vector>

#include <Basil/Packages/Context.hpp>

namespace basil {

// TODO(sholloway): Test coverage
// TODO(sholloway): Documentation
// TODO(sholloway): Specify subsection of window to capture
class ImageFileCapture {
 public:
    static bool capture(std::filesystem::path savePath);
};

}  // namespace basil
