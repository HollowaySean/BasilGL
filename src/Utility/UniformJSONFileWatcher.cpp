#include "UniformJSONFileWatcher.hpp"

namespace basil {

void UniformJSONFileWatcher::updateModel() {
    if (!publisher) return;

    if (!std::filesystem::exists(filePath)) return;

    auto currentTime = std::filesystem::last_write_time(filePath);
    if (currentTime > timestamp) {
        timestamp = currentTime;

        std::optional<ShaderUniformModel> model =
            FileDataLoader::modelFromJSON(filePath);

        if (model.has_value()) {
            publisher->publishData(model.value());
        }
    }
}

UniformJSONFileWatcher::Builder&
UniformJSONFileWatcher::Builder::withFilePath(
        std::filesystem::path filePath) {
    this->impl->setFilePath(filePath);
    return (*this);
}

UniformJSONFileWatcher::Builder&
UniformJSONFileWatcher::Builder::withPublisher(
        std::shared_ptr<IDataPublisher<ShaderUniformModel>> publisher) {
    this->impl->setPublisher(publisher);
    return (*this);
}

}  // namespace basil
