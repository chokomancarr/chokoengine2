#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

#define TG(v) *(glm::vec3*)&(v).x
#define FG(v) *(Vec3*)&(v)

Quat::Quat(const glm::quat& q) : glm::quat(q) {}

Quat::Quat() : glm::quat() {}

Quat::Quat(float w, Vec3 v) : glm::quat(w, TG(v)) {}

Quat::Quat(float w, float x, float y, float z) : glm::quat(w, x, y, z) {}

Quat Quat::operator *(const Quat& rhs) const{
	return static_cast<Quat>((glm::quat)(*this) * (glm::quat)(rhs));
}

Quat Quat::operator *(const float& rhs) const{
	return static_cast<Quat>((glm::quat)(*this) * rhs);
}

Vec3 Quat::operator *(const Vec3& rhs) const{
	auto t = (glm::quat)(*this) * TG(rhs);
	return FG(t);
}

Mat4x4 Quat::matrix() const {
	return glm::mat4_cast(*this);
}

Quat Quat::normalized() const {
	return static_cast<Quat>(glm::normalize((glm::quat)*this));
}

Quat Quat::inverse() const {
	return static_cast<Quat>(glm::inverse((glm::quat)*this));
}

Quat Quat::identity() {
    return Quat(1, 0, 0, 0);
}

Quat Quat::FromEuler(const Vec3& e) {
	return glm::quat(Math::deg2rad * TG(e));
}

Vec3 Quat::ToEuler(const Quat& q) {
	return *(Vec3*)glm::value_ptr(glm::eulerAngles(q));
}

Quat Quat::FromAxisAngle(Vec3 axis, float angle) {
	axis = axis.normalized();
	float a = Math::deg2rad * angle;
	float factor = (float)sin(a / 2.0);
	// Calculate the x, y and z of the quaternion
	float x = axis.x * factor;
	float y = axis.y * factor;
	float z = axis.z * factor;

	// Calculate the w value by cos( theta / 2 )
	float w = (float)cos(a / 2.0);
	return Quat(w, x, y, z);//.normalized();
}

Quat Quat::LookAt(const Vec3& tarr) {
	const Vec3 fwd = Vec3(0, 0, 1);
	Vec3 tar = tarr.normalized();
	float angle = Math::rad2deg * std::acos(Math::Clamp(tar.dot(fwd), -1.f, 1.f));
	if (angle < 0.0001f) return Quat(1, 0, 0, 0);
	Vec3 axis = tar.cross(fwd);
	Vec3 tr = axis.cross(fwd);
	if (tr.dot(tar) < 0) angle *= -1;
	return FromAxisAngle(axis, angle);
}

//https://gamedev.stackexchange.com/questions/53129/quaternion-look-at-with-up-vector
Quat Quat::LookAt(const Vec3& tarr, const Vec3& up) {
	Vec3 tar = tarr.normalized();
	Vec3 fw = Vec3(0, 0, 1);
	Quat q1 = Quat::identity();
	if (tar.z > 0.999f) { //no rotation required
		;
	}
	else if (tar.z < -0.999f) { //inverse
		q1 = FromAxisAngle(Vec3(0, 1, 0), 180.f);
	}
	else {
		Vec3 axis = tar.cross(fw);
		float angle = Math::rad2deg * std::acos(Math::Clamp(tar.dot(fw), -1.f, 1.f));
		Vec3 tr = axis.cross(fw);
		if (tr.dot(tar) < 0) angle *= -1;
		q1 = FromAxisAngle(axis, angle);
	}

	Vec3 mup = q1 * Vec3::up();
	Vec3 mrt = q1 * Vec3::right();
	Vec3 rt = up.cross(tar).normalized();
	float angle2 = Math::rad2deg * std::acos(Math::Clamp(mrt.dot(rt), -1.f, 1.f));
	if (mup.dot(rt) < 0) angle2 *= -1;
	Quat q2 = FromAxisAngle(tar, angle2);

	return q2 * q1;
}

Quat Quat::Slerp(const Quat& a, const Quat& b, const float t) {
	return glm::slerp(a, b, t);
}

CE_END_NAMESPACE