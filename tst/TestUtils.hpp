#ifndef TST_TESTUTILS_HPP_
#define TST_TESTUTILS_HPP_

#include <string>

#include <catch.hpp>

#include <Basil/Context.hpp>

using basil::BasilContextLock;

// Preprocessor definition to lock test
#define BASIL_LOCK_TEST auto lock = BasilTestUtils::lockBasilTestContext();

class BasilTestUtils {
 public:
    static BasilContextLock lockBasilTestContext() {
        std::string testName = Catch::getResultCapture().getCurrentTestName();
        return BasilContextLock(testName);
    }
};

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

#endif  // TST_TESTUTILS_HPP_
