#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_AnimClip::Value::Value() : empty(true), position(Vec3()), 
		rotation(Quat::identity()), scale(Vec3()) {}

_AnimClip::Value::Value(const Vec3& t, const Quat& r, const Vec3& s)
		: empty(false), position(t), rotation(r), scale(s) {}

_AnimClip::Value _AnimClip::Value::operator +(const Value& v) const{
	return Value(
		position + v.position,
		v.rotation * rotation,
		scale + v.scale
	);
}

_AnimClip::Value _AnimClip::Value::operator *(const float& f) const{
	return Value(
		position * f,
		rotation * f,
		scale * f
	);
}

CE_END_NAMESPACE