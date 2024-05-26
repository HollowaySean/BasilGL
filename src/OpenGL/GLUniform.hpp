#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "GLTexture.hpp"

namespace basil {

/** @brief Supported scalar types for OpenGL uniforms */
using GLUniformScalar = std::variant<bool, float, int, uint>;

/** @brief Supported vector types for OpenGL uniforms */
using GLUniformVector = std::variant<
        std::vector<bool>,
        std::vector<float>,
        std::vector<int>,
        std::vector<uint>
    >;

template<typename T>
concept GLUniformScalarType =
    std::is_convertible_v<std::vector<T>, GLUniformVector>;

template<typename T>
concept GLUniformVectorType =
    std::is_convertible_v<T, GLUniformVector>;

template<typename T>
concept GLUniformType =
    GLUniformScalarType<T> || GLUniformVectorType<T>;

// TODO(sholloway): Documentation
// TODO(sholloway): Refactor to include matrix dimensions
// TODO(sholloway): Refactor GLShaderProgram
struct GLUniform {
    template<GLUniformVectorType T>
    GLUniform(T vectorValue,
            const std::string& name,
            unsigned int count = 1,
            unsigned int length = 0,
            unsigned int width = 1) :
                value(vectorValue),
                name(name),
                count(count),
                length(length),
                width(width) {
        if (length == 0) {
            length = vectorValue.size();
        }
    }

    template<GLUniformScalarType T>
    GLUniform(T scalarValue,
        const std::string& name)
        : GLUniform(std::vector<T>({scalarValue}), name, 1, 1, 1) {}

    GLUniform(std::shared_ptr<IGLTexture> texture,
        const std::string& name)
        : GLUniform(texture->getUniformLocation(), name) {}

    GLUniformVector value;

    /** @brief Name used in OpenGL shader program */
    std::string name;

    unsigned int length;
    unsigned int width;
    unsigned int count;
};

}   // namespace basil
