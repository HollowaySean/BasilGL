#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "GLTexture.hpp"

namespace basil {

/** @brief Union of scalar types for OpenGL uniforms */
using GLUniformScalar = std::variant<bool, float, int, uint>;

/** @brief Union of vector types for OpenGL uniforms */
using GLUniformVector = std::variant<
        std::vector<bool>,
        std::vector<float>,
        std::vector<int>,
        std::vector<uint>
    >;

/** @brief Type which can be cast to GLUniformScalar */
template<typename T>
concept GLUniformScalarType =
    std::is_convertible_v<std::vector<T>, GLUniformVector>;

/** @brief Type which can be cast to GLUniformVector */
template<typename T>
concept GLUniformVectorType =
    std::is_convertible_v<T, GLUniformVector>;

/** @brief Type which can be cast to GLUniformScalar or GLUniformVector */
template<typename T>
concept GLUniformType =
    GLUniformScalarType<T> || GLUniformVectorType<T>;

/** @brief Struct to hold details of OpenGL uniform value */
struct GLUniform {
    /** @brief Create uniform vector or matrix
     *  @param  vectorValue Source of data to set
     *  @param  name        Name of uniform in shader
     *  @param  count       Number of values to set
     *  @param  length      Length of vector, or height of matrix
     *  @param  width       Width of matrix
     *  @tparam Numeric type of value
     */
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

    /** @brief Create uniform scalar
     *  @param  scalarValue Source of data to set
     *  @param  name        Name of uniform in shader
     *  @tparam Numeric type of value
     */
    template<GLUniformScalarType T>
    GLUniform(T scalarValue,
        const std::string& name)
        : GLUniform(std::vector<T>({scalarValue}), name, 1, 1, 1) {}

    /** @brief Create uniform from texture
     *  @param  texture     Pointer to IGLTexture object
     *  @param  name        Name of texture uniform in shader
     */
    GLUniform(std::shared_ptr<IGLTexture> texture,
        const std::string& name)
        : GLUniform(texture->getUniformLocation(), name) {}

    /** @brief Stored uniform value */
    GLUniformVector value;

    /** @brief Name used in OpenGL shader program */
    std::string name;

    /** @brief Length of vector, or height of matrix */
    unsigned int length;

    /** @brief Width of matrix */
    unsigned int width;

    /** @brief Number of values to set */
    unsigned int count;
};

}   // namespace basil
