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

    template<GLUniformType T>
    unsigned int addUniform(T value, const std::string& name) {
        return addUniform(std::make_shared<GLUniformScalar<T>>(value, name));
    }

    template<GLUniformType T>
    unsigned int addUniform(T* value, const std::string& name) {
        return addUniform(std::make_shared<GLUniformPointer<T>>(value, name));
    }

    template<GLUniformType T>
    unsigned int addUniform(std::vector<T> value, const std::string& name) {
        return addUniform(std::make_shared<GLUniformVector<T>>(value, name));
    }

    unsigned int addUniform(std::shared_ptr<IGLTexture> value, const std::string& name) {
        return addUniform(std::make_shared<GLUniformTexture>(value, name));
    }

    /** @brief Updates uniform in model
     *  @param uniform      New GLUniform
     *  @param uniformID    ID of uniform to set
     *  @returns            Whether uniform with ID was found */
    bool setUniform(
        std::shared_ptr<GLUniform>, unsigned int uniformID);

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

    bool setUniformValue(std::shared_ptr<IGLTexture> value, unsigned int uniformID) {
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
        Builder& withUniform(std::shared_ptr<GLUniform> uniform);

        // TODO(sholloway): Build GLUniform in builder method
    };

 private:
    std::map<std::string, unsigned int> uniformIDs;
    std::map<unsigned int, std::shared_ptr<GLUniform>> uniforms;

    static inline unsigned int nextID = 0;
};

}   // namespace basil
