#include <catch.hpp>

#include "Utility/UniformJSONFileWatcher.hpp"

using basil::IDataPublisher;
using basil::ShaderUniformModel;
using basil::UniformJSONFileWatcher;

class TestUniformPublisher : public IDataPublisher<ShaderUniformModel> {
 public:
    void publishData(const ShaderUniformModel& dataModel) override {
        hasPublishedData = true;
        IDataPublisher::publishData(dataModel);
    }

    bool hasPublishedData = false;
};

TEST_CASE("Data_UniformJSONFileWatcher_onLoop") {
    auto publisher = std::make_shared<TestUniformPublisher>();
    auto path = std::filesystem::path(TEST_DIR) /
        "Utility/assets/test.json";

    SECTION("Updates based on file modification time") {
        auto watcher = UniformJSONFileWatcher(path);
        watcher.setPublisher(publisher);

        CHECK_FALSE(publisher->hasPublishedData);
        watcher.onLoop();
        CHECK(publisher->hasPublishedData);
    }
}

TEST_CASE("Data_UniformJSONFileWatcher_Builder") {
    auto publisher = std::make_shared<TestUniformPublisher>();
    auto path = std::filesystem::path(TEST_DIR) /
        "Utility/assets/test.json";

    SECTION("Builds correctly") {
        auto watcher = UniformJSONFileWatcher::Builder()
            .withFilePath(path)
            .withPublisher(publisher)
            .build();

        CHECK(watcher->filePath == path);
        CHECK(watcher->publisher == publisher);
    }
}
