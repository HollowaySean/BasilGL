#pragma once

#include <filesystem>
#include <string>

#include <catch.hpp>

#include "Context/BasilContextLock.hpp"
#include "Window/IPane.hpp"

using basil::BasilContextLock;
using basil::PaneProps;

// Preprocessor definition to lock test
#define BASIL_LOCK_TEST auto lock = BasilContextLock();

// Commonly used definitions
inline std::filesystem::path vertexPath =
    std::filesystem::path(TEST_DIR) / "Window/assets/test.vert";
inline std::filesystem::path fragmentPath =
    std::filesystem::path(TEST_DIR) / "Window/assets/test.frag";
inline std::filesystem::path shaderToyPath =
    std::filesystem::path(TEST_DIR) / "Window/assets/test.shadertoy";
inline std::filesystem::path badFragmentPath =
    std::filesystem::path(TEST_DIR) / "Window/assets/invalid-frag";
inline std::filesystem::path validPath =
    std::filesystem::path(TEST_DIR) / "Window/assets/valid-file.txt";
inline std::filesystem::path invalidPath =
    std::filesystem::path(TEST_DIR) / "Window/assets/missing-file.txt";

inline const char* validShaderCode = "#version 330 core\nvoid main() {}\0";

inline const PaneProps testPaneProps = {
    .width = 20,
    .height = 10,
    .xOffset = 5,
    .yOffset = 2
};


