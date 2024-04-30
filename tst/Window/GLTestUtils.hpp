#ifndef TST_WINDOW_GLTESTUTILS_HPP_
#define TST_WINDOW_GLTESTUTILS_HPP_

#include <filesystem>
#include <string>

#include <catch.hpp>

#include <Basil/Context.hpp>

using basil::BasilContextLock;

// Preprocessor definition to lock test
#define BASIL_LOCK_TEST auto lock = BasilContextLock();

// Commonly used definitions
inline std::filesystem::path vertexPath =
    std::filesystem::path(TEST_DIR) / "Window/assets/test.vert";
inline std::filesystem::path fragmentPath =
    std::filesystem::path(TEST_DIR) / "Window/assets/test.frag";
inline std::filesystem::path validPath =
    std::filesystem::path(TEST_DIR) / "Window/assets/valid-file.txt";
inline std::filesystem::path invalidPath =
    std::filesystem::path(TEST_DIR) / "Window/assets/missing-file.txt";

inline const char* validShaderCode = "#version 330 core\nvoid main() {}\0";

#endif  // TST_WINDOW_GLTESTUTILS_HPP_
