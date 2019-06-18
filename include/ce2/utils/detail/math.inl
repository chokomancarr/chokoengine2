#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <typename T, typename F>
T Math::Lerp(const T& a, const T& b, const F& t) {
    return a + (b - a) * t;
}

template <typename T>
T Math::ILerp(const T& a, const T& b, const T& c) {
    return (c - a) / (b - a);
}

template <typename T>
T Math::Clamp(const T& t, const T& a, const T& b) {
	return std::min(std::max(t, a), b);
}

CE_END_NAMESPACE