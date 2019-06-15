#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Mat4x4 : public glm::mat4 {
public:
    Mat4x4();
    Mat4x4(float v);
    Mat4x4(float a, float b, float c, float d, 
            float e, float f, float g, float h, 
            float i, float j, float k, float l, 
            float m, float n, float o, float p);

    Mat4x4(const glm::mat4& rhs);

    static Mat4x4 identity();
    static Mat4x4 translation(const Vec3& t);
    static Mat4x4 rotation(const Quat& q);
    static Mat4x4 rotation(const Vec3& e);
    static Mat4x4 scale(const Vec3& s);
};

CE_END_NAMESPACE