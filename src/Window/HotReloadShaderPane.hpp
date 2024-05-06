#ifndef SRC_WINDOW_HOTRELOADSHADERPANE_HPP_
#define SRC_WINDOW_HOTRELOADSHADERPANE_HPP_

#include <memory>
#include <filesystem>
#include <utility>

#include "GLShaderPane.hpp"

// TODO(sholloway):
//  - Documentation
//  - Standardize function name for builder

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
        Builder& withPaneProps(PaneProps paneProps);
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

}  // namespace basil

#endif  // SRC_WINDOW_HOTRELOADSHADERPANE_HPP_
