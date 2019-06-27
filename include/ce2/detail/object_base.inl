#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <class T>
void DeleteObject(Ref<T>& obj) {
    if (!!obj)
        obj->_deleted = true;
}

CE_END_NAMESPACE