#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Vec2 : public glm::vec2 {
public:
	Vec2();
	Vec2(float v);
	Vec2(float x, float y);
	Vec2(const glm::vec2& q);

	Vec2 operator *(const Vec2& rhs);
	Vec2 operator +(const Vec2& rhs);
	Vec2 operator -(const Vec2& rhs);

	Vec2& operator +=(const Vec2& rhs);
	Vec2& operator -=(const Vec2& rhs);

	Vec2 normalized();

	static Vec2 left();
	static Vec2 right();
	static Vec2 up();
	static Vec2 down();
};

CE_END_NAMESPACE