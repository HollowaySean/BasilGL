#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <Basil/Packages/Context.hpp>

#include "GLUniform.hpp"

namespace basil {

/** @brief Class which manages the caching and setting of OpenGL uniform
 *  values within a shader program. */
class GLProgramUniformManager : private IBasilContextConsumer {
 public:
    /** @brief Initialize GLProgramUniformManager */
    GLProgramUniformManager() : programID(0) {}

    /** @brief Set ID attached to GLShaderProgram */
    void setProgramID(unsigned int programID) {
        this->programID = programID;
    }

    /** @brief Add or change a uniform */
    void setUniform(std::shared_ptr<GLUniform> uniform);

    /** @brief Add or change a texture uniform
     *  @note  Pointers to IGLTexture are saved to map to
     *         prevent easily falling out of scope. */
    void setTextureSource(std::shared_ptr<GLUniformTexture> uniform);

    /** @brief Update uniforms in shader program based on cache */
    void applyCachedUniforms();

#ifndef TEST_BUILD

 private:
#endif
    unsigned int programID;

    int getUniformLocation(const std::string& uniform);
    void setUniformAt(std::shared_ptr<GLUniform> uniform, int location);
    void setUniformWithoutCache(std::shared_ptr<GLUniform> uniform);

    template<GLUniformSourceType T>
    void setUniformVectorOrMatrix(
        GLUniformSource<T> source,
        unsigned int width, unsigned int length,
        unsigned int count, int location);

    void cacheUniform(std::shared_ptr<GLUniform> uniform);

    std::map<std::string, std::shared_ptr<GLUniform>> uniformCache;
    std::map<std::string, std::shared_ptr<IGLTexture>> textureMap;
    std::set<std::string> errorHistory;


    Logger& logger = Logger::get();

    LOGGER_FORMAT LOG_UNIFORM_FAILURE =
        "Shader Program (ID{:02}) - Could not get location for uniform "
        "with name \"{}\".";
};

}  // namespace basil
