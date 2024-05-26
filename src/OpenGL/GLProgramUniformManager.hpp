#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <Basil/Packages/Context.hpp>

#include "GLUniform.hpp"

namespace basil {

// TODO(sholloway): Documentation
class GLProgramUniformManager : private IBasilContextConsumer {
 public:
    GLProgramUniformManager() : programID(0) {}

    void setProgramID(unsigned int programID) {
        this->programID = programID;
    }

    void setUniform(const GLUniform& uniform);

    void setTextureSource(std::shared_ptr<IGLTexture> texture,
        const GLUniform& uniform);

    void applyCachedUniforms();

#ifndef TEST_BUILD

 private:
#endif
    unsigned int programID;

    int getUniformLocation(const std::string& uniform);
    void setUniformAt(const GLUniform& uniform, int location);
    void setUniformWithoutCache(const GLUniform& uniform);

    template<GLUniformVectorType T>
    void setUniformVectorOrMatrix(T value,
        unsigned int count, int location,
        unsigned int length, unsigned int width) {}

    void cacheUniform(const GLUniform& uniform);

    std::map<std::string, GLUniform> uniformCache;
    std::map<std::string, std::shared_ptr<IGLTexture>> textureMap;
    std::set<std::string> errorHistory;


    Logger& logger = Logger::get();

    LOGGER_FORMAT LOG_UNIFORM_FAILURE =
        "Shader Program (ID{:02}) - Could not get location for uniform "
        "with name \"{}\".";
};

}  // namespace basil
