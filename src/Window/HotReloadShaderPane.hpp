#pragma once

#include <filesystem>
#include <memory>

#include <Basil/Packages/Builder.hpp>

#include "GLShaderPane.hpp"
#include "IPane.hpp"

namespace basil {

/** @brief Pane which automatically reloads modified fragment shaders. */
class HotReloadShaderPane : public GLShaderPane,
                            public IBuildable<HotReloadShaderPane> {
 public:
    /** @brief Initialize from file path. */
    HotReloadShaderPane(
        PaneProps paneProps, std::filesystem::path shaderFilePath);

    /** @brief Set path of shader file. */
    void setFilePath(std::filesystem::path shaderFilePath);

    /** @brief Check for file update and draw to screen. */
    void const draw() override;

    /** @brief Builder pattern for HotReloadShaderPane. */
    class Builder : public IBuilder<HotReloadShaderPane> {
     public:
        /** @brief Build with fragment shader at given file path. */
        Builder& fromFilePath(std::filesystem::path shaderFilePath);
    };

#ifndef TEST_BUILD

 private:
#endif
    void updateShader();
    void useDefaultShader();

    friend class IBuilder<HotReloadShaderPane>;
    HotReloadShaderPane();

    std::filesystem::path filePath;
    std::filesystem::file_time_type timestamp
        = std::filesystem::file_time_type::min();

    std::shared_ptr<GLVertexShader> defaultVert
        = GLVertexShader::noOpShader();
    std::shared_ptr<GLFragmentShader> defaultFrag
        = GLFragmentShader::debugShader();
};

}   // namespace basil
