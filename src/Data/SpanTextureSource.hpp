#pragma once

#include <span>

#include <Basil/Packages/Builder.hpp>

#include "ITextureSource.hpp"

namespace basil {

/** @brief Template class which implements ITextureSource with
 *  std::span type object as data source.
 *
 *  @tparam T Numeric type contained in span.
 *  @tparam dimension Dimensionality of texture.
 *  @tparam channels Number of color channels in texture.
 */
template<typename T, int dimension = 2, int channels = 4>
class SpanTextureSource : public ITextureSource<dimension>,
                          public IBuildable<
                            SpanTextureSource<T, dimension, channels>> {
    static_assert(channels > 0 && channels <= 4);

 public:
    /** @brief Creates empty container. */
    SpanTextureSource() {
        this->format = GLTextureFormat::getTextureFormat<T, channels>();
    }

    /** @brief Creates container from std::span source. */
    explicit SpanTextureSource(std::span<T> source) : source(source) {
        this->format = GLTextureFormat::getTextureFormat<T, channels>();
    }

    /** @brief Pointer to const T* data source, as const void*. */
    const void* data() override {
        return source.data();
    }

    /** @brief Sets std::span data source. */
    void setSource(const std::span<T>& source) {
        this->source = source;
    }

    /** @returns std::span container for data. */
    std::span<T>& getSource() { return source; }

    class Builder: public IBuilder<SpanTextureSource<T, dimension, channels>> {
     public:
        /** @brief Creates TextureSource from std::span object. */
        Builder& withSource(std::span<T> source) {
            this->impl->setSource(source);
            return (*this);
        }
    };

#ifndef TEST_BUILD

 private:
#endif
    std::span<T> source;
};

}   // namespace basil
