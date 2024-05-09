#include <catch.hpp>

#include "Data/PubSubTestUtils.hpp"
#include "Widget/UniformJSONFileWatcher.hpp"

using basil::TestUniformSubscriber;
using basil::UniformJSONFileWatcher;

TEST_CASE("Data_UniformJSONFileWatcher_onLoop") {
    auto subscriber = std::make_shared<TestUniformSubscriber>();
    auto path = std::filesystem::path(TEST_DIR) /
        "Widget/assets/test.json";

    SECTION("Updates based on file modification time") {
        auto watcher = UniformJSONFileWatcher(path);
        watcher.subscribe(subscriber);

        CHECK_FALSE(subscriber->hasReceivedData);
        watcher.onLoop();
        CHECK(subscriber->hasReceivedData);
    }
}

TEST_CASE("Data_UniformJSONFileWatcher_Builder") {
    auto subscriber = std::make_shared<TestUniformSubscriber>();
    auto path = std::filesystem::path(TEST_DIR) /
        "Widget/assets/test.json";

    SECTION("Builds correctly") {
        auto watcher = UniformJSONFileWatcher::Builder()
            .withFilePath(path)
            .build();

        CHECK(watcher->filePath == path);
    }
}
