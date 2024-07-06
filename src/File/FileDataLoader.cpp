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

    auto model = std::make_shared<ShaderUniformModel>();

    if (data.empty()) {
        logger.log(
            fmt::format(LOG_JSON_EMPTY, filePath.c_str()),
            LogLevel::WARN);

        return std::nullopt;
    }

    // Check if uniform definition exists
    if (data.contains("uniforms")) {
        // Read data into model
        logger.log("Reading shader uniforms from JSON file.", LogLevel::DEBUG);
        auto uniformData = data.at("uniforms");

        model = addUniforms<float>(model, uniformData);
        model = addUniforms<int>(model, uniformData);
        model = addUniforms<unsigned int>(model, uniformData);
        model = addUniforms<bool>(model, uniformData);

    } else {
        logger.log(
            fmt::format(LOG_UNIFORMS_MISSING, filePath.c_str()),
            LogLevel::DEBUG);
    }

    // Check if texture definition exists
    if (data.contains("textures")) {
        // Read data into model
        logger.log("Reading texture paths from JSON file.", LogLevel::DEBUG);
        auto textureData = data.at("textures");

        auto basePath = filePath.parent_path();
        model = addTextures(model, textureData, basePath);

    } else {
        logger.log(
            fmt::format(LOG_TEXTURES_MISSING, filePath.c_str()),
            LogLevel::DEBUG);
    }

    // Check if texture definition exists
    if (data.contains("cubemaps")) {
        // Read data into model
        logger.log("Reading cubemap paths from JSON file.", LogLevel::DEBUG);
        auto cubemapData = data.at("cubemaps");

        auto basePath = filePath.parent_path();
        model = addCubemaps(model, cubemapData, basePath);

    } else {
        logger.log(
            fmt::format(LOG_CUBEMAP_MISSING, filePath.c_str()),
            LogLevel::DEBUG);
    }

    return std::optional(std::move(*model));
}

std::shared_ptr<ShaderUniformModel>
FileDataLoader::addTextures(
        std::shared_ptr<ShaderUniformModel> model,
        json json,
        std::filesystem::path basePath) {
    for (auto& [index, texInfo] : json.items()) {
        // Verify required fields
        if (!texInfo.contains("name")) {
            logger.log(
                fmt::format(LOG_FIELD_MISSING,
                    "Texture", index.c_str(), "name"),
                LogLevel::WARN);
            return model;
        }
        std::string name = texInfo.at("name");

        if (!texInfo.contains("path")) {
            logger.log(
                fmt::format(LOG_FIELD_MISSING,
                    "Texture", index.c_str(), "path"),
                LogLevel::WARN);
            return model;
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
        if (!cubeInfo.contains("name")) {
            logger.log(
                fmt::format(LOG_FIELD_MISSING,
                    "Cubemap", index.c_str(), "name"),
                LogLevel::WARN);
            return model;
        }
        std::string name = cubeInfo.at("name");

        if (!cubeInfo.contains("paths")) {
            logger.log(
                fmt::format(LOG_FIELD_MISSING,
                    "Cubemap", index.c_str(), "paths"),
                LogLevel::WARN);
            return model;
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

}  // namespace basil
