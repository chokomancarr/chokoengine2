#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Vec2 {
public:
	Vec2(const float v = 0);
	Vec2(const float x, float y);

	float x, y;

	float operator [](const int i) const;

	Vec2 operator -() const;

	Vec2 operator +(const Vec2& rhs) const;
	Vec2 operator -(const Vec2& rhs) const;
	Vec2 operator *(const Vec2& rhs) const;
	Vec2 operator /(const Vec2& rhs) const;

	bool operator ==(const Vec2& rhs) const;
	bool operator !=(const Vec2& rhs) const;

	Vec2& operator +=(const Vec2& rhs);
	Vec2& operator -=(const Vec2& rhs);
	Vec2& operator *=(const Vec2& rhs);
	Vec2& operator /=(const Vec2& rhs);

	Vec2 normalized() const;
	float length() const;
	float length2() const;
	float dot(const Vec2& rhs) const;

	std::string str() const;

	static Vec2 left();
	static Vec2 right();
	static Vec2 up();
	static Vec2 down();
};

CE_END_NAMESPACE