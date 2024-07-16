#include "SphereGenerator.hpp"

namespace basil::raytracer {

SphereGenerator::SphereGenerator() : IBasilWidget({
        "SphereGenerator",
        ProcessOrdinal::MAIN,
        ProcessPrivilege::NONE,
        WidgetPubSubPrefs::PUBLISH_ONLY
    }) {}

void SphereGenerator::onStart() {
    setUpUniforms();
    generateSpheres();
}

void SphereGenerator::setSphereGridSize(int gridSize) {
    this->sphereGridSize = gridSize;

    uniforms.setUniformValue(gridSize * gridSize, countID);

    setUpUniforms();
}

void SphereGenerator::setUpUniforms() {
    int numSpheres = sphereGridSize * sphereGridSize;
    positionID  = uniforms.addUniform(
        std::make_shared<GLUniformVector<float>>(
            positions, "spherePositions", 3, 1, numSpheres));
    sizeID      = uniforms.addUniform(
        std::make_shared<GLUniformVector<float>>(
            sizes, "sphereSizes", 1, 1, numSpheres));
    albedoID    = uniforms.addUniform(
        std::make_shared<GLUniformVector<float>>(
            albedo, "sphereAlbedo", 3, 1, numSpheres));
    specularID  = uniforms.addUniform(
        std::make_shared<GLUniformVector<float>>(
            specular, "sphereSpecular", 3, 1, numSpheres));
    countID     = uniforms.addUniform(
            numSpheres, "sphereCount");
}

void SphereGenerator::generateSpheres() {
    generatePositions();
    generateSizes();
    generateAlbedo();
    generateSpecular();

    publishData(DataMessage(uniforms));
}

void SphereGenerator::generatePositions() {
    positions.clear();

    for (int x = 0; x < sphereGridSize; x++) {
        for (int z = 0; z < sphereGridSize; z++) {
            float xPos = 2*x - sphereGridSize + random(rng) + 0.5f;
            float yPos = 0.5f + random(rng);
            float zPos = 2*z + random(rng) - 0.5f;

            positions.push_back(xPos);
            positions.push_back(yPos);
            positions.push_back(zPos);
        }
    }

    uniforms.setUniformValue(positions, positionID);
}

void SphereGenerator::generateSizes() {
    sizes.clear();

    for (int i = 0; i < sphereGridSize*sphereGridSize; i++) {
        float size = (maxSize - minSize) * random(rng) + minSize;
        sizes.push_back(size);
    }

    uniforms.setUniformValue(sizes, sizeID);
}

void SphereGenerator::generateAlbedo() {
    albedo.clear();

    for (int i = 0; i < 3*sphereGridSize*sphereGridSize; i++) {
        float value = (maxAlbedo - minAlbedo) * random(rng) + minAlbedo;
        albedo.push_back(value);
    }

    uniforms.setUniformValue(albedo, albedoID);
}

void SphereGenerator::generateSpecular() {
    specular.clear();

    for (int i = 0; i < 3*sphereGridSize*sphereGridSize; i++) {
        float value = (maxSpecular - minSpecular) * random(rng) + minSpecular;
        specular.push_back(value);
    }

    uniforms.setUniformValue(specular, specularID);
}

}  // namespace basil::raytracer
