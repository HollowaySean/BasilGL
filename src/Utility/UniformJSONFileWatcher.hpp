#ifndef SRC_UTILITY_UNIFORMJSONFILEWATCHER_HPP_
#define SRC_UTILITY_UNIFORMJSONFILEWATCHER_HPP_

#include <filesystem>
#include <memory>

#include <Basil/Builder.hpp>
#include <Basil/Data.hpp>
#include <Basil/Logging.hpp>
#include <Basil/Process.hpp>

namespace basil {

/** @brief Utility class to hot reload shader uniforms from JSON file */
class UniformJSONFileWatcher : public IProcess,
                               public IBuildable<UniformJSONFileWatcher> {
 public:
    /** @brief Initialize with given file path of JSON file. */
    explicit UniformJSONFileWatcher(std::filesystem::path filePath)
        : filePath(filePath) {}

    /** @brief Set publisher to receive data. */
    void setPublisher(
            std::shared_ptr<IDataPublisher<ShaderUniformModel>> setPublisher) {
        this->publisher = setPublisher;
    }

    /** @brief Set path to JSON file. */
    void setFilePath(std::filesystem::path setFilePath) {
        this->filePath = setFilePath;
    }

    /** @brief Reload JSON uniforms on process main loop. */
    void onLoop() override { updateModel(); }

    /** @brief Builder pattern for UniformJSONFileWatcher */
    class Builder : public IBuilder<UniformJSONFileWatcher> {
     public:
        /** @brief Set JSON file path in builder. */
        Builder& withFilePath(
            std::filesystem::path filePath);

        /** @brief Set IDataPublisher in builder. */
        Builder& withPublisher(
            std::shared_ptr<IDataPublisher<ShaderUniformModel>> publisher);
    };

#ifndef TEST_BUILD

 private:
#endif
    friend class IBuilder<UniformJSONFileWatcher>;
    UniformJSONFileWatcher() = default;

    void updateModel();

    std::filesystem::path filePath;
    std::filesystem::file_time_type timestamp
        = std::filesystem::file_time_type::min();
    std::shared_ptr<IDataPublisher<ShaderUniformModel>> publisher
        = nullptr;
};

}  // namespace basil

#endif  // SRC_UTILITY_UNIFORMJSONFILEWATCHER_HPP_
