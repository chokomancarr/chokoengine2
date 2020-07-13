#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Quat {
public:
    Quat();
    Quat(float w, Vec3 v);
    Quat(float w, float x, float y, float z);

	float x, y, z, w;

	Quat operator *(const Quat& rhs) const;
	Quat operator *(const float& rhs) const;
	Vec3 operator *(const Vec3& rhs) const;

	Mat4x4 matrix() const;

	Quat normalized() const;
	Quat inverse() const;

    static Quat identity();
	static Quat FromEuler(const Vec3& e);
	static Vec3 ToEuler(const Quat& q);
	static Quat FromAxisAngle(Vec3 axis, float angle);
	static Quat LookAt(const Vec3& tar);
	static Quat LookAt(const Vec3& tar, const Vec3& up);

	static Quat Slerp(const Quat& a, const Quat& b, const float t);
};

CE_END_NAMESPACE