#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

namespace Math {

	const float deg2rad = 0.0174532925f;
	const float rad2deg = 57.295779513f;

    template <typename T, typename F>
    T Lerp(const T& a, const T& b, const F& t);

    template <typename T>
    T ILerp(const T& a, const T& b, const T& c);

	template <typename T>
	T Clamp(const T& t, const T& a, const T& b);
}

CE_END_NAMESPACE

#include "detail/math.inl"