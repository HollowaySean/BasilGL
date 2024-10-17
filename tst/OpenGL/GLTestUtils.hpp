#pragma once

#include <filesystem>
#include <string>

#include <catch.hpp>

#include "Context/BasilContextLock.hpp"
#include "Window/IPane.hpp"

using basil::BasilContextLock;
using basil::ViewArea;

// Preprocessor definition to lock test
#define BASIL_LOCK_TEST auto lock = BasilContextLock();

// Commonly used definitions
inline std::filesystem::path vertexPath =
    std::filesystem::path(TEST_DIR) / "OpenGL/assets/test.vert";
inline std::filesystem::path fragmentPath =
    std::filesystem::path(TEST_DIR) / "OpenGL/assets/test.frag";
inline std::filesystem::path shaderToyPath =
    std::filesystem::path(TEST_DIR) / "OpenGL/assets/test.shadertoy";
inline std::filesystem::path badFragmentPath =
    std::filesystem::path(TEST_DIR) / "OpenGL/assets/invalid-frag";
inline std::filesystem::path validPath =
    std::filesystem::path(TEST_DIR) / "OpenGL/assets/valid-file.txt";
inline std::filesystem::path invalidPath =
    std::filesystem::path(TEST_DIR) / "OpenGL/assets/missing-file.txt";

inline const char* validShaderCode = "#version 330 core\nvoid main() {}\0";
