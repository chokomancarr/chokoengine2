#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Vec3 {
public:
	float x, y, z;

	Vec3(float v = 0);
	Vec3(float x, float y, float z);
	Vec3(Vec2 v, float z);

	float operator [](const int i) const;

	Vec3 operator -() const;

	Vec3 operator +(const Vec3& rhs) const;
	Vec3 operator -(const Vec3& rhs) const;
	Vec3 operator *(const Vec3& rhs) const;
	Vec3 operator /(const Vec3& rhs) const;

	bool operator ==(const Vec3& rhs) const;
	bool operator !=(const Vec3& rhs) const;

	Vec3& operator +=(const Vec3& rhs);
	Vec3& operator -=(const Vec3& rhs);
	Vec3& operator *=(const Vec3& rhs);
	Vec3& operator /=(const Vec3& rhs);

	float dot(const Vec3& v) const;
	Vec3 cross(const Vec3& v) const;

	Vec3 normalized() const;
	float length() const;
	float length2() const;

	std::string str() const;

	static Vec3 left();
	static Vec3 right();
	static Vec3 up();
	static Vec3 down();
	static Vec3 front();
	static Vec3 back();
};

Vec3 operator* (const glm::mat3& m, const Vec3& v);

CE_END_NAMESPACE