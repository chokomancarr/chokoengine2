#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Quat : public glm::quat {
	Quat(const glm::quat& q);

public:
    Quat();
    Quat(float w, Vec3 v);
    Quat(float w, float x, float y, float z);

	Quat operator *(const Quat& rhs);
	Vec3 operator *(const Vec3& rhs);

	Mat4x4 matrix() const;

	Quat normalized();

    static Quat identity();
	static Quat FromEuler(const Vec3& e);
	static Quat FromAxisAngle(Vec3 axis, float angle);
	static Quat LookAt(const Vec3& tar);
	static Quat LookAt(const Vec3& tar, const Vec3& up);
};

CE_END_NAMESPACE