#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

template <typename T>
T* EOverlay::GetInstance() {
    static_assert(std::is_base_of<EOverlay, T>::value, "");
    if (!instance) {
        instance = std::make_shared<T>();
        EOverlayManager::overlays.push_back(instance);
    }
    return (T*)instance.get();
}

CE_END_ED_NAMESPACE