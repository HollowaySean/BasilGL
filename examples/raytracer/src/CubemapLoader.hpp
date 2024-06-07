#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include <Basil/App.hpp>

using basil::DataMessage;
using basil::GLTextureCubemap;
using basil::IBasilWidget;
using basil::ShaderUniformModel;

// TODO(sholloway): Toggle mouse look
// TODO(sholloway): Better way about aspect ratio
// TODO(sholloway): More cleanup
// TODO(sholloway): Remove callback on destructor

namespace basil::raytracer {

class CubemapLoader : public IBasilWidget {
 public:
    CubemapLoader(std::filesystem::path path, const std::string& name) :
            IBasilWidget({
                    "CubemapLoader",
                    basil::ProcessOrdinal::LATE,
                    basil::ProcessPrivilege::NONE,
                    basil::WidgetPubSubPrefs::PUBLISH_ONLY
                }) {
        cubemap = GLTextureCubemap::Builder()
            .fromFile(path / "right.jpg",   GL_TEXTURE_CUBE_MAP_POSITIVE_X)
            .fromFile(path / "left.jpg",    GL_TEXTURE_CUBE_MAP_NEGATIVE_X)
            .fromFile(path / "top.jpg",     GL_TEXTURE_CUBE_MAP_POSITIVE_Y)
            .fromFile(path / "bottom.jpg",  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y)
            .fromFile(path / "front.jpg",   GL_TEXTURE_CUBE_MAP_POSITIVE_Z)
            .fromFile(path / "back.jpg",    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
            .build();
        uniformModel.addUniform(cubemap, name);
    }

    void onStart() override {
        publishData(DataMessage(uniformModel));
    }

    void onLoop() override {}

 private:
    std::shared_ptr<GLTextureCubemap> cubemap;

    ShaderUniformModel uniformModel;
};

}  // namespace basil::raytracer
