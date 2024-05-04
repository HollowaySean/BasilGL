#include "HotReloadShaderPane.hpp"

namespace basil {

HotReloadShaderPane::HotReloadShaderPane() {
    setupGLBuffers();
    currentShaderProgram = std::make_shared<GLShaderProgram>();
    currentShaderProgram->setVertexShader(defaultVert);
}

HotReloadShaderPane::HotReloadShaderPane(PaneProps paneProps,
        std::filesystem::path shaderFilePath) : HotReloadShaderPane() {
    setFilePath(filePath);
}

void HotReloadShaderPane::setFilePath(std::filesystem::path shaderFilePath) {
    filePath = shaderFilePath;
    updateShader();
}

void const HotReloadShaderPane::draw() {
    updateShader();
    this->GLShaderPane::draw();
}

void HotReloadShaderPane::updateShader() {
    if (!std::filesystem::exists(filePath)) {
        useDefaultShader();
        return;
    }

    auto writeTime = std::filesystem::last_write_time(filePath);
    if (writeTime > timestamp) {
        timestamp = writeTime;
        auto fragmentFromFile = std::make_shared<GLFragmentShader>(filePath);

        if (fragmentFromFile->hasCompiledSuccessfully()) {
            currentShaderProgram->setFragmentShader(fragmentFromFile);
        } else {
            useDefaultShader();
        }
    }
}

void HotReloadShaderPane::useDefaultShader() {
    if (currentShaderProgram->getFragmentShader() != defaultFrag) {
        currentShaderProgram->setFragmentShader(defaultFrag);
    }
}

HotReloadShaderPane::Builder&
HotReloadShaderPane::Builder::fromFilePath(
        std::filesystem::path shaderFilePath) {
    this->impl->setFilePath(shaderFilePath);
    return (*this);
}

}  // namespace basil
