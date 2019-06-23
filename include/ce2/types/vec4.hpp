#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Vec4 : public glm::vec4 {

public:
	Vec4();
	Vec4(float v);
	Vec4(float x, float y, float z, float w);
	Vec4(Vec3 v, float w);

	Vec4(const glm::vec4& q);

	Vec4 operator *(const Vec4& rhs);

	Vec4 normalized();

	static Vec4 left(float w = 0);
	static Vec4 right(float w = 0);
	static Vec4 up(float w = 0);
	static Vec4 down(float w = 0);
	static Vec4 front(float w = 0);
	static Vec4 back(float w = 0);
};

CE_END_NAMESPACE