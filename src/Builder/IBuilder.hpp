#pragma once

#include <memory>
#include <utility>

namespace basil {

/** @brief Interface which creates Builder
 *  pattern for a given class. */
template<class T>
class IBuilder {
 public:
    IBuilder() : impl(new T()) {}

    /** @returns Unique_ptr to implemented object. */
    virtual std::shared_ptr<T> build() {
        return std::move(impl);
    }

 protected:
    std::unique_ptr<T> impl;
};

}   // namespace basil
