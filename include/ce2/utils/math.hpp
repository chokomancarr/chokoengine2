#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

namespace Math {

	const float deg2rad = 0.0174532925f;
	const float rad2deg = 57.295779513f;

	/* Linear interpolate from A to B at T
	 * if unclamped, out-of-bounds values are extrapolated
	 */
    template <typename T, typename F>
    T Lerp(const T& a, const T& b, const F& t, bool clamped = false);

	/* Similar to Lerp, but wrap values around
	 * 0~360
	 */
	template <typename T, typename F>
	T LerpAngle(const T&a, const T& b, const F& t);

	/* Get the linear interpolated T from A to B
	 * given the value C
	 */
    template <typename T>
	T ILerp(const T& a, const T& b, const T& c, bool clamped = false);

	/* Bound t between a and b via minmax
	 */
	template <typename T>
	T Clamp(const T& t, const T& a, const T& b);

	/* Bound t between a and b via repeating
	 */
	template <typename T>
	T Repeat(const T& a, const T& b, const T& t);
}

CE_END_NAMESPACE

#include "detail/math.inl"