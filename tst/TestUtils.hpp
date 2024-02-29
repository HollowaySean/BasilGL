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

#endif  // TST_TESTUTILS_HPP_
