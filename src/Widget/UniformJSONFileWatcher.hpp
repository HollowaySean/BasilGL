#ifndef SRC_WIDGET_UNIFORMJSONFILEWATCHER_HPP_
#define SRC_WIDGET_UNIFORMJSONFILEWATCHER_HPP_

#include <filesystem>
#include <memory>

#include <Basil/App.hpp>

namespace basil {

/** @brief Utility class to hot reload shader uniforms from JSON file */
class UniformJSONFileWatcher : public IBasilWidget,
                               public IBuildable<UniformJSONFileWatcher> {
 public:
    /** @brief Initialize with given file path of JSON file. */
    explicit UniformJSONFileWatcher(std::filesystem::path filePath);

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
    };

#ifndef TEST_BUILD

 protected:
#endif
    friend class IBuilder<UniformJSONFileWatcher>;
    UniformJSONFileWatcher();

    void updateModel();

    std::filesystem::path filePath;
    std::filesystem::file_time_type timestamp
        = std::filesystem::file_time_type::min();
};

}  // namespace basil

#endif  // SRC_WIDGET_UNIFORMJSONFILEWATCHER_HPP_
