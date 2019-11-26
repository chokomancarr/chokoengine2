#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <typename T>
Ref<T> _Object::get_shared() const {
    return Ref<T>(Object(
		std::const_pointer_cast<_Object>(
            shared_from_this()
        )
    ));
};

CE_END_NAMESPACE