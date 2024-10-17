#pragma once

#include <memory>
#include <random>
#include <vector>

#include <Basil/Packages/Data.hpp>
#include <Basil/Packages/Widget.hpp>

namespace basil::raytracer {

/** @brief Widget which generates the grid of spheres and updates uniforms. */
class SphereGenerator : public IBasilWidget,
                        public IBuildable<SphereGenerator> {
 public:
    /** @brief Initialize widget. */
    SphereGenerator();

    /** @brief Set up uniforms and generate sphere list. */
    void onStart() override;

    /** @brief No-op on loop. */
    void onLoop() override {}

    /**
     *  @brief Set the width of the sphere grid. Total number of spheres
     *  rendered is gridSize^2.
     */
    void setSphereGridSize(int gridSize);

    /**
     * @param min Minimum radius of sphere.
     * @param max Maximum radius of sphere.
     */
    void setSizeLimits(float min, float max) {
        this->minSize = min;
        this->maxSize = max;
    }

    /**
     * @param min Minimum albedo value for each channel.
     * @param max Maximum albedo value for each channel.
     */
    void setAlbedoLimits(float min, float max) {
        this->minAlbedo = min;
        this->maxAlbedo = max;
    }

    /**
     * @param min Minimum specular value for each channel.
     * @param max Maximum specular value for each channel.
     */
    void setSpecularLimits(float min, float max) {
        this->minSpecular = min;
        this->maxSpecular = max;
    }

    /** @brief Generate randomized spheres and publish uniforms. */
    void generateSpheres();

    /** @brief Builder pattern for SphereGenerator class. */
    class Builder : public IBuilder<SphereGenerator> {
     public:
        /**
         *  @brief Set the width of the sphere grid. Total number of spheres
         *  rendered is gridSize^2.
         */
        Builder& withGridSize(int gridSize) {
            this->impl->setSphereGridSize(gridSize);
            return (*this);
        }

        /**
         * @param min Minimum radius of sphere.
         * @param max Maximum radius of sphere.
         */
        Builder& withSizeLimits(float min, float max) {
            this->impl->setSizeLimits(min, max);
            return (*this);
        }

        /**
         * @param min Minimum albedo value for each channel.
         * @param max Maximum albedo value for each channel.
         */
        Builder& withAlbedoLimits(float min, float max) {
            this->impl->setAlbedoLimits(min, max);
            return (*this);
        }

        /**
         * @param min Minimum specular value for each channel.
         * @param max Maximum specular value for each channel.
         */
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
