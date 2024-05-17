#pragma once

#include "IBuilder.hpp"

namespace basil {

/** @brief Interface to automatically implement IBuilder
 *  in derived class. */
template<class T>
class IBuildable {
 public:
    class Builder : public IBuilder<T> {};
};

}   // namespace basil
