#include <catch.hpp>

#include <Basil/Utility.hpp>

using basil::LambdaProcess;

TEST_CASE("Utility_LambdaProcess_onLoop") {
    bool hasRun = false;
    std::function<void()> lambda = [&]() { hasRun = true; };

    SECTION("Runs a given lambda during main loop") {
        auto process = LambdaProcess(lambda);
        process.onLoop();

        CHECK(hasRun);
    }
}
