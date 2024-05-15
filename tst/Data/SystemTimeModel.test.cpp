#include <catch.hpp>

#include "Data/SystemTimeModel.hpp"

using basil::SystemTimeModel;

TEST_CASE("Data_SystemTimeModel_frameRate") {
    auto model = SystemTimeModel();

    model.frameNumber = 10;
    model.timeSinceStart = std::chrono::seconds(10);

    CHECK(model.frameRate() == 1.);
}
