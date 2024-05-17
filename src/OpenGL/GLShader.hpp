#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include <Basil/Packages/Context.hpp>
#include <Basil/Packages/Logging.hpp>

namespace basil {

/** @brief Container class for OpenGL shader. */
class GLShader : private IBasilContextConsumer {
 public:
    /** @return ID value assigned from OpenGL. */
    virtual GLuint getID() const { return ID; }

    /** @brief Sets shader code from file, and compiles. */
    virtual void setShader(std::filesystem::path path) = 0;

    /** @brief Sets shader code from string, and compiles. */
    virtual void setShader(const std::string& shaderCode) = 0;

    /** @returns Boolean indicating if code has compiled. */
    bool hasCompiledSuccessfully() { return hasCompiled; }

    /** @brief Destructor, tears down OpenGL shader code. */
    ~GLShader();

#ifndef TEST_BUILD

 protected:
#endif
    enum ShaderType { FRAGMENT, VERTEX };

    GLShader() = default;
    GLShader(std::filesystem::path path, ShaderType type);
    GLShader(const std::string &shaderCode, ShaderType type);

    void setShaderWithType(std::filesystem::path path, ShaderType type);
    void setShaderWithType(const std::string& shaderCode, ShaderType type);

    void destroyShader();

    GLuint ID = 0;

    static inline const std::string NO_OP_VERTEX_CODE =
        "#version 450 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "out vec2 TexCoord;\n"
        "void main() {\n"
        "gl_Position = vec4(aPos, 1.0);\n"
        "TexCoord = aTexCoord; }";

    static inline const std::string DEBUG_FRAGMENT_CODE =
        "#version 450 core\n"
        "out vec4 FragColor;\n"
        "uniform float patternSize = 50.;\n"
        "uniform vec4 highColor = vec4(0.5, 0.0, 0.5, 1.0);\n"
        "uniform vec4 lowColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
        "void main() {\n"
        "vec2 coord = floor(gl_FragCoord.xy / patternSize);\n"
        "float mask = mod(coord.x + mod(coord.y, 2.0), 2.0);\n"
        "FragColor = mask * highColor; }";

    static inline const std::string SHADERTOY_PREFIX =
        "#version 450 core\n"
        "out vec4 st_translate_fragColor;\n"
        "uniform vec3 iResolution;\n"
        "uniform vec4 iMouse;\n"
        "#line 1\n";
        // "uniform float iTime;\n"
        // "uniform float iTimeDelta;\n"
        // "uniform float iFrame;\n"
        // "uniform vec4 iDate;\n"

    static inline const std::string SHADERTOY_SUFFIX =
        "void main() { mainImage(st_translate_fragColor, gl_FragCoord.xy); }";

#ifndef TEST_BUILD

 private:
#endif
    Logger& logger = Logger::get();

    bool hasCompiled = false;

    std::string rawShaderCode;

    void getShaderFromFile(std::filesystem::path path);
    void getShaderFromString(const std::string &shaderCode);
    void compileShader(ShaderType type);

    LOGGER_FORMAT LOG_READ_FAILURE =
        "Unable to read shader file at path {} "
        "See error message:";
    LOGGER_FORMAT LOG_READ_SUCCESS =
        "Shader file read successfully.";

    LOGGER_FORMAT LOG_TRANSLATE_SHADERTOY =
        "Translating Shadertoy file to portable GLSL.";

    LOGGER_FORMAT LOG_COMPILE_FAILURE =
        "Shader (ID{:02}) - Unable to compile {} shader. "
        "See OpenGL info log:";
    LOGGER_FORMAT LOG_COMPILE_SUCCESS =
        "Shader (ID{:02}) - Compiled {} shader successfully.";
    LOGGER_FORMAT LOG_DESTROY =
        "Shader (ID{:02}) - Shader deleted.";
};

/** @brief GLShader implementation for vertex shader. */
class GLVertexShader : public GLShader {
 public:
    /** @brief Default constructor. Does not compile shader. */
    GLVertexShader() = default;

    /** @brief Pass-through vertex shader. */
    static std::shared_ptr<GLVertexShader> noOpShader();

    /** @brief Create vertex shader from file at path. */
    explicit GLVertexShader(std::filesystem::path path);

    /** @brief Create vertex shader from shader code. */
    explicit GLVertexShader(const std::string &shaderCode);

    /** @brief Sets shader code from file, and compiles. */
    void setShader(std::filesystem::path path) override;

    /** @brief Sets shader code from string, and compiles. */
    void setShader(const std::string& shaderCode) override;
};

/** @brief GLShader implementation for fragment shader. */
class GLFragmentShader : public GLShader {
 public:
    /** @brief Default constructor. Does not compile shader. */
    GLFragmentShader() = default;

    /** @brief Debug pattern fragment shader. */
    static std::shared_ptr<GLFragmentShader> debugShader();

    /** @brief Create fragment shader from file at path. */
    explicit GLFragmentShader(std::filesystem::path path);

    /** @brief Create fragment shader from shader code. */
    explicit GLFragmentShader(const std::string &shaderCode);

    /** @brief Sets shader code from file, and compiles. */
    void setShader(std::filesystem::path path) override;

    /** @brief Sets shader code from string, and compiles. */
    void setShader(const std::string& shaderCode) override;
};

}   // namespace basil
