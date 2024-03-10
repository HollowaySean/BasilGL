#ifndef SRC_DATA_SPANTEXTURESOURCE_HPP_
#define SRC_DATA_SPANTEXTURESOURCE_HPP_

#include <span>

#include "ITextureSource.hpp"

namespace basil {

template<typename T, int dimension = 2, int channels = 4>
class SpanTextureSource : public ITextureSource<dimension> {
    static_assert(channels > 0 && channels <= 4);

 public:
    SpanTextureSource() {
        this->format = TextureFormat::getTextureFormat<T, channels>();
    }

    explicit SpanTextureSource(std::span<T> source) : source(source) {
        this->format = TextureFormat::getTextureFormat<T, channels>();
    }

    const void* data() override {
        return source.data();
    }

    void setSource(const std::span<T>& source) {
        this->source = source;
    }

    std::span<T>& getSource() { return source; }

#ifndef TEST_BUILD

 private:
#endif
    std::span<T> source;
};

}  // namespace basil

#endif  // SRC_DATA_SPANTEXTURESOURCE_HPP_
