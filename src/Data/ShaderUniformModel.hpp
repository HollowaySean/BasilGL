#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <Basil/Packages/Builder.hpp>

#include "OpenGL/GLUniform.hpp"

namespace basil {

/** @brief Data model used to maintain
 *  uniforms for GLShaderProgram objects. */
class ShaderUniformModel : public IBuildable<ShaderUniformModel> {
 public:
    /** @brief Add uniform to model
     *  @param uniform      GLUniform struct
     *  @returns            UID for string-less lookup */
    unsigned int addUniform(std::shared_ptr<GLUniform> uniform);

    /** @brief Add scalar uniform to model
     *  @tparam             Data type of OpenGL uniform
     *  @param value        Value of uniform scalar
     *  @param name         Uniform name in shader
     *  @returns            UID for string-less lookup */
    template<GLUniformType T>
    unsigned int addUniform(
            T value,
            const std::string& name) {
        return addUniform(std::make_shared<GLUniformScalar<T>>(value, name));
    }

    /** @brief Add pointer uniform to model
     *  @tparam             Data type of OpenGL uniform
     *  @param value        Pointer to uniform data
     *  @param name         Uniform name in shader
     *  @returns            UID for string-less lookup */
    template<GLUniformType T>
    unsigned int addUniform(
            T* value,
            const std::string& name) {
        return addUniform(std::make_shared<GLUniformPointer<T>>(value, name));
    }

    /** @brief Add std::vector uniform to model
     *  @tparam             Data type of OpenGL uniform
     *  @param value        Vector of uniform values
     *  @param name         Uniform name in shader
     *  @returns            UID for string-less lookup */
    template<GLUniformType T>
    unsigned int addUniform(
            std::vector<T> value,
            const std::string& name) {
        return addUniform(std::make_shared<GLUniformVector<T>>(value, name));
    }

    /** @brief Add texture uniform to model
     *  @param value        Pointer to IGLTexture object
     *  @param name         Uniform name in shader
     *  @returns            UID for string-less lookup */
    unsigned int addUniform(
            std::shared_ptr<IGLTexture> value,
            const std::string& name) {
        return addUniform(std::make_shared<GLUniformTexture>(value, name));
    }

    /** @brief Updates uniform in model
     *  @param uniform      New GLUniform
     *  @param uniformID    ID of uniform to set
     *  @returns            Whether uniform with ID was found */
    bool setUniform(
        std::shared_ptr<GLUniform> uniform, unsigned int uniformID);

    /** @brief Updates value of uniform in model to data pointer
     *  @tparam             Data type of OpenGL uniform
     *  @param value        Pointer to uniform data
     *  @param uniformID    ID of uniform to set
     *  @returns            Whether uniform with ID was found */
    template<GLUniformType T>
    bool setUniformValue(T* value, unsigned int uniformID) {
        if (uniforms.contains(uniformID)) {
            std::shared_ptr<GLUniform> base = uniforms.at(uniformID);
            uniforms.at(uniformID) =
                std::make_shared<GLUniformPointer<T>>(value, *base);
            return true;
        }
        return false;
    }

    /** @brief Updates value of uniform in model to scalar value
     *  @tparam             Data type of OpenGL uniform
     *  @param value        Scalar value of uniform
     *  @param uniformID    ID of uniform to set
     *  @returns            Whether uniform with ID was found */
    template<GLUniformType T>
    bool setUniformValue(T value, unsigned int uniformID) {
        if (uniforms.contains(uniformID)) {
            std::shared_ptr<GLUniform> base = uniforms.at(uniformID);
            uniforms.at(uniformID) =
                std::make_shared<GLUniformScalar<T>>(value, *base);
            return true;
        }
        return false;
    }

    /** @brief Updates value of uniform in model to std::vector value
     *  @tparam             Data type of OpenGL uniform
     *  @param value        Vector of uniform values
     *  @param uniformID    ID of uniform to set
     *  @returns            Whether uniform with ID was found */
    template<GLUniformType T>
    bool setUniformValue(std::vector<T> value, unsigned int uniformID) {
        if (uniforms.contains(uniformID)) {
            std::shared_ptr<GLUniform> base = uniforms.at(uniformID);
            uniforms.at(uniformID) =
                std::make_shared<GLUniformVector<T>>(value, *base);
            return true;
        }
        return false;
    }

    /** @brief Updates value of uniform in model to texture location
     *  @param value        Pointer to IGLTexture object
     *  @param uniformID    ID of uniform to set
     *  @returns            Whether uniform with ID was found */
    bool setUniformValue(
            std::shared_ptr<IGLTexture> value, unsigned int uniformID) {
        if (uniforms.contains(uniformID)) {
            std::shared_ptr<GLUniform> base = uniforms.at(uniformID);
            uniforms.at(uniformID) =
                std::make_shared<GLUniformTexture>(value, *base);
            return true;
        }
        return false;
    }

    /** @brief Gets value of uniform with identifier, if found */
    std::optional<std::shared_ptr<GLUniform>> getUniform(
        const std::string& uniformName) const;

    /** @brief Gets value of uniform with ID, if found */
    std::optional<std::shared_ptr<GLUniform>> getUniform(
        unsigned int uniformID) const;

    /** @returns Reference to map containing all uniforms in model */
    const std::map<unsigned int, std::shared_ptr<GLUniform>>& getUniforms()
        const { return uniforms; }

    /** @brief Builder pattern for ShaderUniformModel */
    class Builder : public IBuilder<ShaderUniformModel> {
     public:
        /** @brief Adds uniform to model */
        Builder& withUniform(std::shared_ptr<GLUniform> uniform) {
            this->impl->addUniform(uniform);
            return (*this);
        }

        /** @brief Adds pointer uniform to model */
        template<GLUniformType T>
        Builder& withUniform(T* value, const std::string& name) {
            this->impl->addUniform(value, name);
            return (*this);
        }

        /** @brief Adds scalar uniform to model */
        template<GLUniformType T>
        Builder& withUniform(T value, const std::string& name) {
            this->impl->addUniform(value, name);
            return (*this);
        }

        /** @brief Adds vector uniform to model */
        template<GLUniformType T>
        Builder& withUniform(
                std::vector<T> value,
                const std::string& name) {
            this->impl->addUniform(value, name);
            return (*this);
        }

        /** @brief Adds texture uniform to model */
        Builder& withUniform(
                std::shared_ptr<IGLTexture> value,
                const std::string& name) {
            this->impl->addUniform(value, name);
            return (*this);
        }
    };

 private:
    std::map<std::string, unsigned int> uniformIDs;
    std::map<unsigned int, std::shared_ptr<GLUniform>> uniforms;

    static inline unsigned int nextID = 0;
};

}   // namespace basil
