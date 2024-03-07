#include "MetricsObserver.hpp"

namespace basil {

void MetricsObserver::setBufferSize(unsigned int newBufferSize) {
    if (newBufferSize > 0) {
        bufferSize = newBufferSize;
    }
}

}  // namespace basil
