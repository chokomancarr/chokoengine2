#pragma once
#include "../ui_float.hpp"

CE_BEGIN_NAMESPACE

template <typename T>
UI::Float<T>::Float(T on, T off, bool ison, float sp)
    : _enabled(ison), settime(0), lerpsp(sp), val_on(on), val_off(off) {}

template <typename T>
void UI::Float<T>::set(bool b) {
    _enabled = b;
    settime = Time::millis();
}

template <typename T>
UI::Float<T>::operator T() const {
    float t = (Time::millis() - settime) * lerpsp * Time::delta();
    return Math::Lerp(val_off, val_on, _enabled ? t : 1 - t, true);
}

CE_END_NAMESPACE