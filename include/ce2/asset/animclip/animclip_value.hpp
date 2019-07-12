#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

struct _AnimClip::Value {
	bool empty;

	Vec3 position;
	Quat rotation;
	Vec3 scale;

	Value();
	Value(const Vec3&, const Quat&, const Vec3&);

	/* Operators to support lerp
	 */
	Value operator +(const Value&) const;
	Value operator *(const float&) const;
};

CE_END_NAMESPACE