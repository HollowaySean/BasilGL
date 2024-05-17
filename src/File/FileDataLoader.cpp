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
            LogLevel::WARN);
    }


    // Check if texture definition exists
    if (data.contains("textures")) {
        // Read data into model
        logger.log("Reading texture paths from JSON file.", LogLevel::DEBUG);
        auto textureData = data.at("textures");

        auto basePath = filePath.parent_path();
        model = addTexture(model, textureData, basePath);

    } else {
        logger.log(
            fmt::format(LOG_UNIFORMS_MISSING, filePath.c_str()),
            LogLevel::WARN);
    }

    return std::optional(std::move(*model));
}

std::shared_ptr<ShaderUniformModel>
FileDataLoader::addTexture(
        std::shared_ptr<ShaderUniformModel> model,
        json json,
        std::filesystem::path basePath) {
    for (auto& [key, value] : json.items()) {
        if (value.is_string()) {
            std::filesystem::path path = value;

            if (path.is_relative()) {
                path = basePath / value;
            }

            logger.log(
                fmt::format(LOG_TEXTURE_ADDED,
                    key.c_str(), path.c_str()),
                LogLevel::DEBUG);

            auto texture = GLTexture2D::Builder()
                .fromFile(path)
                .build();
            model->addTexture(texture, key);
        }
    }

    return model;
}

}  // namespace basil
