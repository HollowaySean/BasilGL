#include <catch.hpp>

#include "File/FileDataLoader.hpp"

using basil::FileDataLoader;
using basil::GLUniformDirectVector;
using basil::Logger;
using basil::LogLevel;

TEST_CASE("File_FileDataLoader_modelFromJSON") {
    Logger& logger = Logger::get();
    auto basePath = std::filesystem::path(TEST_DIR) / "File/assets";

    SECTION("Fails if file does not exist") {
        auto filePath = basePath / "missing.json";

        auto result = FileDataLoader::modelFromJSON(filePath);
        CHECK_FALSE(result.has_value());
        CHECK(logger.getLastLevel() == LogLevel::ERROR);
    }

    SECTION("Fails if file can not be parsed") {
        auto filePath = basePath / "malformed.txt";

        auto result = FileDataLoader::modelFromJSON(filePath);
        CHECK_FALSE(result.has_value());
        CHECK(logger.getLastLevel() == LogLevel::ERROR);
    }

    SECTION("Fails quietly if file is empty") {
        auto filePath = basePath / "empty.json";

        auto result = FileDataLoader::modelFromJSON(filePath);
        CHECK_FALSE(result.has_value());
        CHECK(logger.getLastLevel() == LogLevel::WARN);
    }

    SECTION("Fails quietly if file does not contain related fields list") {
        auto filePath = basePath / "missing-fields.json";

        auto result = FileDataLoader::modelFromJSON(filePath);
        CHECK(result.has_value());
        CHECK(result.value().getUniforms().size() == 0);
        CHECK(result.value().getTextures().size() == 0);
        CHECK(logger.getLastLevel() == LogLevel::DEBUG);
    }

    SECTION("Succeeds for properly formatted JSON") {
        auto filePath = basePath / "correct.json";

        auto result = FileDataLoader::modelFromJSON(filePath);
        REQUIRE(result.has_value());

        auto model = result.value();
        CHECK(model.getUniform("test1").value().value
            == GLUniformDirectVector(std::vector<float>({0.8f})));
        CHECK(model.getUniform("test2").value().value
            == GLUniformDirectVector(std::vector<float>({ 0.0f, 456.789f })));
        CHECK(model.getUniform("test3").value().value
            == GLUniformDirectVector(std::vector<int>({true})));
        CHECK(model.getUniform("test4").value().value
            == GLUniformDirectVector(std::vector<int>({ false, true })));
        CHECK(model.getUniform("test5").value().value
            == GLUniformDirectVector(std::vector<uint>({ 9 })));
        CHECK(model.getUniform("test6").value().value
            == GLUniformDirectVector(std::vector<uint>({ 10 })));

        CHECK_FALSE(model.getUniform("test7").has_value());

        REQUIRE(model.getTextureSource("testTexture").has_value());
        CHECK(model.getTextureSource("testTexture").value());
    }
}

TEST_CASE("File_FileDataLoader_TypeMap") {
    SECTION("Returns correct type keys") {
        CHECK(FileDataLoader::TypeMap<float>::key == "float");
        CHECK(FileDataLoader::TypeMap<int>::key == "int");
        CHECK(FileDataLoader::TypeMap<unsigned int>::key == "unsigned int");
        CHECK(FileDataLoader::TypeMap<bool>::key == "bool");
    }

    SECTION("Correctly interprets types") {
        nlohmann::json json = nlohmann::json::parse(R"(
            {
                "float" : 1.5,
                "int" : -5,
                "unsigned int" : 10,
                "bool" : true
            }
        )");

        CHECK(FileDataLoader::TypeMap<float>::isCorrectType(
            json["float"]));
        CHECK(FileDataLoader::TypeMap<float>::isCorrectType(
            json["int"]));
        CHECK(FileDataLoader::TypeMap<int>::isCorrectType(
            json["int"]));
        CHECK(FileDataLoader::TypeMap<int>::isCorrectType(
            json["unsigned int"]));
        CHECK(FileDataLoader::TypeMap<unsigned int>::isCorrectType(
            json["unsigned int"]));
        CHECK(FileDataLoader::TypeMap<bool>::isCorrectType(
            json["bool"]));
    }
}
