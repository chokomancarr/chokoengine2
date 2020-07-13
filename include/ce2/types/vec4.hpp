#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Vec4 {
public:
	Vec4(float v = 0);
	Vec4(float x, float y, float z, float w);
	Vec4(Vec2 a, Vec2 b);
	Vec4(Vec3 v, float w);

	float x, y, z, w;

	explicit operator Vec2() const;
	explicit operator Vec3() const;

	float& operator [](const int i);

	Vec4 operator -() const;

	Vec4 operator +(const Vec4& rhs) const;
	Vec4 operator -(const Vec4& rhs) const;
	Vec4 operator *(const Vec4& rhs) const;
	Vec4 operator /(const Vec4& rhs) const;

	bool operator ==(const Vec4& rhs) const;
	bool operator !=(const Vec4& rhs) const;

	Vec4& operator +=(const Vec4& rhs);
	Vec4& operator -=(const Vec4& rhs);
	Vec4& operator *=(const Vec4& rhs);
	Vec4& operator /=(const Vec4& rhs);

	float dot(const Vec4& v) const;

	Vec4 normalized() const;
	float length() const;
	float length2() const;

	std::string str() const;

	static Vec4 left(float w = 0);
	static Vec4 right(float w = 0);
	static Vec4 up(float w = 0);
	static Vec4 down(float w = 0);
	static Vec4 front(float w = 0);
	static Vec4 back(float w = 0);
};

CE_END_NAMESPACE