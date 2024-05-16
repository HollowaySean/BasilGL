#include "UniformJSONFileWatcher.hpp"

namespace basil {

UniformJSONFileWatcher::UniformJSONFileWatcher() : IBasilWidget({
        "UniformJSONFileWatcher",
        ProcessOrdinal::EARLY,
        ProcessPrivilege::NONE,
        WidgetPubSubPrefs::PUBLISH_ONLY
    }) {}

UniformJSONFileWatcher::UniformJSONFileWatcher(std::filesystem::path filePath)
        : UniformJSONFileWatcher() {
    setFilePath(filePath);
}

void UniformJSONFileWatcher::updateModel() {
    if (!std::filesystem::exists(filePath)) return;

    auto currentTime = std::filesystem::last_write_time(filePath);
    if (currentTime > timestamp) {
        timestamp = currentTime;

        std::optional<ShaderUniformModel> model =
            FileDataLoader::modelFromJSON(filePath);

        if (model.has_value()) {
            auto message = DataMessage(model.value());
            publishData(message);
        }
    }
}

UniformJSONFileWatcher::Builder&
UniformJSONFileWatcher::Builder::withFilePath(
        std::filesystem::path filePath) {
    this->impl->setFilePath(filePath);
    return (*this);
}

}  // namespace basil
