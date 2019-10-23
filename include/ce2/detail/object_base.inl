#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <typename T>
std::shared_ptr<T> _Object::get_shared() const {
    return std::static_pointer_cast<T>(
        std::const_pointer_cast<_Object>(
            shared_from_this()
        )
    );
};

CE_END_NAMESPACE