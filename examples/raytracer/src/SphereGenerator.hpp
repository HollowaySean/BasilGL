#pragma once

#include <memory>
#include <random>
#include <vector>

#include <Basil/Packages/Data.hpp>
#include <Basil/Packages/Widget.hpp>

namespace basil::raytracer {

class SphereGenerator : public IBasilWidget,
                        public IBuildable<SphereGenerator> {
 public:
    SphereGenerator();

    void onStart() override;

    void onLoop() override {}

    void setSphereGridSize(int gridSize);

    void setSizeLimits(float min, float max) {
        this->minSize = min;
        this->maxSize = max;
    }

    void setAlbedoLimits(float min, float max) {
        this->minAlbedo = min;
        this->maxAlbedo = max;
    }

    void setSpecularLimits(float min, float max) {
        this->minSpecular = min;
        this->maxSpecular = max;
    }

    void generateSpheres();

    class Builder : public IBuilder<SphereGenerator> {
     public:
        Builder& withGridSize(int gridSize) {
            this->impl->setSphereGridSize(gridSize);
            return (*this);
        }

        Builder& withSizeLimits(float min, float max) {
            this->impl->setSizeLimits(min, max);
            return (*this);
        }

        Builder& withAlbedoLimits(float min, float max) {
            this->impl->setAlbedoLimits(min, max);
            return (*this);
        }

        Builder& withSpecularLimits(float min, float max) {
            this->impl->setSpecularLimits(min, max);
            return (*this);
        }
    };

 private:
    void setUpUniforms();

    void generatePositions();
    void generateSizes();
    void generateAlbedo();
    void generateSpecular();

    float maxSize = 1.0f;
    float minSize = 0.2f;

    float maxAlbedo = 1.0f;
    float minAlbedo = 0.0f;

    float maxSpecular = 0.4f;
    float minSpecular = 0.0f;

    int sphereGridSize = 1;

    unsigned int positionID = -1;
    unsigned int sizeID     = -1;
    unsigned int albedoID   = -1;
    unsigned int specularID = -1;
    unsigned int countID    = -1;

    std::vector<float> positions;
    std::vector<float> sizes;
    std::vector<float> albedo;
    std::vector<float> specular;

    ShaderUniformModel uniforms = ShaderUniformModel();

    std::default_random_engine rng;
    std::uniform_real_distribution<float> random =
        std::uniform_real_distribution<float>(0.0f, 1.0f);
};

}  // namespace basil::raytracer
