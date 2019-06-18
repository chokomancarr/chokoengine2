#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Math {
public:
	static const float deg2rad;
	static const float rad2deg;

    template <typename T, typename F>
    static T Lerp(const T& a, const T& b, const F& t);

    template <typename T>
    static T ILerp(const T& a, const T& b, const T& c);

	template <typename T>
	static T Clamp(const T& t, const T& a, const T& b);
};

CE_END_NAMESPACE

#include "detail/math.inl"