#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <test.hpp>

TEST_CASE("Testing this dummy function first time", "[testFunction]") {
    REQUIRE(testFunction(1, 2) == -1);
    REQUIRE(testFunction(0, 0) == 0);
}


TEST_CASE("Testing dummy function again", "[testFunction2]") {
    REQUIRE(testFunction(2, 1) == 3);
    REQUIRE(testFunction(0, 0) == 0);
}
