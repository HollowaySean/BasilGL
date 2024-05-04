#ifndef SRC_WINDOW_HOTRELOADSHADERPANE_HPP_
#define SRC_WINDOW_HOTRELOADSHADERPANE_HPP_

#include <memory>
#include <filesystem>

#include "GLShaderPane.hpp"

namespace basil {

class HotReloadShaderPane : public GLShaderPane,
                            public IBuildable<HotReloadShaderPane> {
 public:
    HotReloadShaderPane(
        PaneProps paneProps, std::filesystem::path shaderFilePath);

    void setFilePath(std::filesystem::path shaderFilePath);

    void const draw() override;

    class Builder : public IBuilder<HotReloadShaderPane> {
     public:
        Builder& fromFilePath(std::filesystem::path shaderFilePath);
    };

 private:
    void updateShader();
    void useDefaultShader();

    friend class IBuilder<HotReloadShaderPane>;
    HotReloadShaderPane();

    std::filesystem::path filePath;
    std::filesystem::file_time_type timestamp
        = std::filesystem::file_time_type::min();

    std::shared_ptr<GLFragmentShader> defaultFrag =
        std::make_shared<GLFragmentShader>(GLFragmentShader::debugShader());
    std::shared_ptr<GLVertexShader> defaultVert =
        std::make_shared<GLVertexShader>(GLVertexShader::noOpShader());
};

}  // namespace basil

#endif  // SRC_WINDOW_HOTRELOADSHADERPANE_HPP_
