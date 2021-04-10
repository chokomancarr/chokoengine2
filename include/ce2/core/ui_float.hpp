#pragma once
#include "ui.hpp"

CE_BEGIN_NAMESPACE

template <typename T>
class UI::Float {
    bool _enabled = true;
    uint64_t settime = 0;
public:
    Float() = default;
    Float(T on, T off, bool ison, float sp = 1);

    bool enabled() const { return _enabled; }

    float lerpsp = 1;
    T val_on = T(), val_off = T();

    void set(bool);

    operator T() const;
};

CE_END_NAMESPACE

#include "detail/ui_float.inl"