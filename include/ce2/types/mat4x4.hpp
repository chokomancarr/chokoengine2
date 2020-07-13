#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Mat4x4 {
	std::array<float, 16> data;
public:
    Mat4x4(float v = 1);
    Mat4x4(float a, float b, float c, float d, 
            float e, float f, float g, float h, 
            float i, float j, float k, float l, 
            float m, float n, float o, float p);

	float& operator [](const int i);

	Vec4 operator *(const Vec4& rhs) const;
	Mat4x4 operator *(const Mat4x4& rhs) const;

	Mat4x4 inverse() const;

    static Mat4x4 Identity();
    static Mat4x4 Translation(const Vec3& t);
    static Mat4x4 Rotation(const Quat& q);
    static Mat4x4 Rotation(const Vec3& e);
    static Mat4x4 Scale(const Vec3& s);
	static Mat4x4 FromTRS(const Vec3& t, const Quat& r, const Vec3& s);
};

CE_END_NAMESPACE