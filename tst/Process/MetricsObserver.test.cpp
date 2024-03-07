#include <catch.hpp>

#include <Basil/Process.hpp>

using basil::MetricsObserver;

TEST_CASE("Process_MetricsObserver_setBufferSize") {
    MetricsObserver metrics = MetricsObserver();

    SECTION("Sets correctly if greater than zero") {
        REQUIRE(metrics.bufferSize == 1);
        metrics.setBufferSize(5);
        REQUIRE(metrics.bufferSize == 5);
    }

    SECTION("No op if set to zero") {
        REQUIRE(metrics.bufferSize == 1);
        metrics.setBufferSize(0);
        REQUIRE(metrics.bufferSize == 1);
    }
}
