#include <optional>
#include <fstream>
#include <utility>

#include "FileDataLoader.hpp"

#include "OpenGL/GLTexture.hpp"

namespace basil {

template<> const std::string_view
FileDataLoader::TypeMap<float>::key = "float";
template<> const std::string_view
FileDataLoader::TypeMap<int>::key = "int";
template<> const std::string_view
FileDataLoader::TypeMap<unsigned int>::key = "unsigned int";
template<> const std::string_view
FileDataLoader::TypeMap<bool>::key = "bool";

template<>
bool FileDataLoader::TypeMap<float>::isCorrectType(json json) {
    return json.is_number_float() || json.is_number_integer();
}
template<>
bool FileDataLoader::TypeMap<int>::isCorrectType(json json) {
    return json.is_number_integer();
}
template<>
bool FileDataLoader::TypeMap<unsigned int>::isCorrectType(json json) {
    return json.is_number_unsigned();
}
template<>
bool FileDataLoader::TypeMap<bool>::isCorrectType(json json) {
    return json.is_boolean();
}

std::optional<ShaderUniformModel>
FileDataLoader::modelFromJSON(std::filesystem::path filePath) {
    // Parse JSON from file
    json data;
    std::ifstream dataFile;
    dataFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        dataFile.open(filePath);
        data = json::parse(dataFile);

        logger.log(
            fmt::format(LOG_READ_SUCCESS),
            LogLevel::DEBUG);
    } catch(std::ifstream::failure& error) {
        logger.log(
            fmt::format(LOG_READ_FAILURE, filePath.c_str()),
            LogLevel::ERROR);
        logger.log(
            strerror(errno),
            LogLevel::ERROR);

        return std::nullopt;
    } catch(json::parse_error& error) {
        logger.log(
            fmt::format(LOG_PARSE_FAILURE, filePath.c_str(), error.byte),
            LogLevel::ERROR);
        logger.log(
            error.what(),
            LogLevel::ERROR);

        return std::nullopt;
    }

    if (data.empty()) {
        logger.log(
            fmt::format(LOG_JSON_EMPTY, filePath.c_str()),
            LogLevel::WARN);

        return std::nullopt;
    }

    auto model = std::make_shared<ShaderUniformModel>();

    // Check if uniform definition exists
    if (data.contains("uniforms")) {
        // Read data into model
        logger.log(
            fmt::format(LOG_READING_FIELD, "shader uniforms"),
            LogLevel::DEBUG);
        auto uniformData = data.at("uniforms");

        model = addUniforms(model, uniformData);

    } else {
        logger.log(
            fmt::format(LOG_FIELD_MISSING, "uniforms", filePath.c_str()),
            LogLevel::DEBUG);
    }

    // Check if texture definition exists
    if (data.contains("textures")) {
        // Read data into model
        logger.log(
            fmt::format(LOG_READING_FIELD, "texture paths"),
            LogLevel::DEBUG);
        auto textureData = data.at("textures");

        auto basePath = filePath.parent_path();
        model = addTextures(model, textureData, basePath);

    } else {
        logger.log(
            fmt::format(LOG_FIELD_MISSING, "textures", filePath.c_str()),
            LogLevel::DEBUG);
    }

    // Check if texture definition exists
    if (data.contains("cubemaps")) {
        // Read data into model
        logger.log(
            fmt::format(LOG_READING_FIELD, "cubemap paths"),
            LogLevel::DEBUG);
        auto cubemapData = data.at("cubemaps");

        auto basePath = filePath.parent_path();
        model = addCubemaps(model, cubemapData, basePath);

    } else {
        logger.log(
            fmt::format(LOG_FIELD_MISSING, "cubemaps", filePath.c_str()),
            LogLevel::DEBUG);
    }

    return std::optional(std::move(*model));
}

std::shared_ptr<ShaderUniformModel>
FileDataLoader::addUniforms(
        std::shared_ptr<ShaderUniformModel> model,
        json json) {
    for (auto& [index, info] : json.items()) {
        // Verify required fields
        if (!verifySubfield(
                info, index, "Uniform", "name")) {
            continue;
        }
        std::string name = info.at("name");

        if (!verifySubfield(
                info, index, "Uniform", "value")) {
            continue;
        }
        auto value = info.at("value");

        std::string type;
        if (info.contains("type")) {
            type = info.at("type");
        } else {
            type = "";
        }

        if (type == TypeMap<bool>::key) {
            model = addUniform<bool>(model, value, name);
        } else if (type == TypeMap<unsigned int>::key) {
            model = addUniform<unsigned int>(model, value, name);
        } else if (type == TypeMap<int>::key) {
            model = addUniform<int>(model, value, name);
        } else {
            model = addUniform<float>(model, value, name);
        }
    }

    return model;
}

std::shared_ptr<ShaderUniformModel>
FileDataLoader::addTextures(
        std::shared_ptr<ShaderUniformModel> model,
        json json,
        std::filesystem::path basePath) {
    for (auto& [index, texInfo] : json.items()) {
        // Verify required fields
        if (!verifySubfield(
                texInfo, index, "Texture", "name")) {
            continue;
        }
        std::string name = texInfo.at("name");

        if (!verifySubfield(
                texInfo, index, "Texture", "path")) {
            continue;
        }
        std::filesystem::path path = texInfo.at("path");

        if (path.is_relative()) {
            path = basePath / path;
        }

        logger.log(
            fmt::format(LOG_TEXTURE_ADDED,
                name, path.string()),
            LogLevel::DEBUG);

        auto texture = GLTexture2D::Builder()
            .fromFile(path)
            .build();
        model->addUniform(texture, name);
    }

    return model;
}

std::shared_ptr<ShaderUniformModel>
FileDataLoader::addCubemaps(
        std::shared_ptr<ShaderUniformModel> model,
        json json,
        std::filesystem::path basePath) {
    for (auto& [index, cubeInfo] : json.items()) {
        // Verify required fields
        if (!verifySubfield(
                cubeInfo, index, "Cubemap", "name")) {
            continue;
        }
        std::string name = cubeInfo.at("name");

        if (!verifySubfield(
                cubeInfo, index, "Cubemap", "paths")) {
            continue;
        }
        auto paths = cubeInfo.at("paths");

        // Get base path if it is defined
        std::filesystem::path cubeBasePath;
        if (paths.contains("base")) {
            cubeBasePath = std::filesystem::path(
                paths.at("base"));
        }
        if (cubeBasePath.is_relative()) {
            cubeBasePath = basePath / cubeBasePath;
        }

        // Create cubemap texture object
        auto cubemapBuilder = GLTextureCubemap::Builder();
        logger.log(
            fmt::format(LOG_CUBEMAP_CREATED, name),
            LogLevel::DEBUG);

        // Loop over faces
        for (auto& [key, value] : paths.items()) {
            GLenum cubeFaceEnum;
            if (!CUBE_FACE_TO_ENUM_MAP.contains(key)) {
                continue;
            } else {
                cubeFaceEnum = CUBE_FACE_TO_ENUM_MAP.at(key);
            }

            std::filesystem::path facePath = value;
            if (facePath.is_relative()) {
                facePath = cubeBasePath / facePath;
            }

            logger.log(
                fmt::format(LOG_CUBEMAP_FACE_ADDED,
                    key.c_str(), facePath.c_str()),
                LogLevel::DEBUG);
            cubemapBuilder.fromFile(facePath, cubeFaceEnum);
        }

        auto cubemap = cubemapBuilder.build();
        model->addUniform(cubemap, name);
    }

    return model;
}

bool FileDataLoader::verifySubfield(json json,
        const std::string& index,
        const std::string& baseField,
        const std::string& subField) {
    if (!json.contains(subField)) {
        logger.log(
            fmt::format(LOG_SUBFIELD_MISSING,
                baseField, index, subField),
            LogLevel::WARN);
        return false;
    }
    return true;
}

}  // namespace basil
