#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Mat3x3 {
	std::array<float, 9> data;
public:
    Mat3x3(float v = 1);
    Mat3x3(float a, float b, float c, 
		float d, float e, float f,
		float g, float h, float i);

	float& operator [](const int i);

	Vec3 operator *(const Vec3& rhs) const;
	Mat3x3 operator *(const Mat3x3& rhs) const;

	Mat3x3 inverse() const;

    static Mat3x3 Identity();
};

CE_END_NAMESPACE