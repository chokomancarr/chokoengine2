#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <typename T, typename F>
T Math::Lerp(const T& a, const T& b, const F& t) {
    return a * (1 - t) + b * t;
}

template <>
Quat Math::Lerp<Quat, float>(const Quat& a, const Quat& b, const float& t);

template <typename T, typename F>
T Math::LerpAngle(const T& a, const T& b, const F& t) {
	T ab = Repeat<T>(-180, 180, b - a);
	ab *= t;
	return Repeat<T>(0, 360, a + ab);
}

template <typename T>
T Math::ILerp(const T& a, const T& b, const T& c) {
    return (c - a) / (b - a);
}

template <typename T>
T Math::Clamp(const T& t, const T& a, const T& b) {
	return std::min(std::max(t, 
		std::min(a, b)), 
		std::max(a, b));
}

template <typename T>
T Math::Repeat(const T& a, const T& b, const T& t) {
	if (t > a && t < b) return t;
	if (b <= a) return a;
	const auto ba = b - a;
	return ba * glm::fract((t - a) / ba) + a;
}

CE_END_NAMESPACE