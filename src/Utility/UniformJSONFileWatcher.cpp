#include "UniformJSONFileWatcher.hpp"

namespace basil {

UniformJSONFileWatcher::UniformJSONFileWatcher(std::filesystem::path filePath)
        : UniformJSONFileWatcher() {
    setFilePath(filePath);
}

UniformJSONFileWatcher::UniformJSONFileWatcher()
    : IBasilWidget(ProcessOrdinal::EARLY, ProcessPrivilege::NONE) {
        this->IProcess::setProcessName("UniformJSONFileWatcher");
}

void UniformJSONFileWatcher::updateModel() {
    if (!std::filesystem::exists(filePath)) return;

    auto currentTime = std::filesystem::last_write_time(filePath);
    if (currentTime > timestamp) {
        timestamp = currentTime;

        std::optional<ShaderUniformModel> model =
            FileDataLoader::modelFromJSON(filePath);

        if (model.has_value()) {
            publishData(model.value());
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
