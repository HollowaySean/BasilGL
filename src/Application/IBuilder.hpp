#ifndef SRC_APPLICATION_IBUILDER_HPP_
#define SRC_APPLICATION_IBUILDER_HPP_

#include <memory>
#include <utility>

namespace basil {

template<class T>
class IBuilder {
 public:
    IBuilder() : impl(new T()) {}

    virtual std::unique_ptr<T> build() {
        return std::move(impl);
    }

 protected:
    std::unique_ptr<T> impl;
};

}  // namespace basil

#endif  // SRC_APPLICATION_IBUILDER_HPP_
