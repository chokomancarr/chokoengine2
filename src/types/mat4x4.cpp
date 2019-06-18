#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Mat4x4::Mat4x4() : glm::mat4() {}

Mat4x4::Mat4x4(float f) : glm::mat4(f) {}

Mat4x4::Mat4x4(float a, float b, float c, float d, 
        float e, float f, float g, float h, 
        float i, float j, float k, float l, 
        float m, float n, float o, float p)
        : glm::mat4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) {}

Mat4x4::Mat4x4(const glm::mat4& m) : glm::mat4(m) {}

Mat4x4 Mat4x4::inverse() const {
	return glm::inverse(*this);
}

Mat4x4 Mat4x4::Identity() {
    return Mat4x4(1);
}

Mat4x4 Mat4x4::Translation(const Vec3& t) {
    return Mat4x4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        t.x, t.y, t.z, 1);
}

Mat4x4 Mat4x4::Rotation(const Quat& q) {
	return q.matrix();
}

Mat4x4 Mat4x4::Rotation(const Vec3& e) {
	return Quat::FromEuler(e).matrix();
}

Mat4x4 Mat4x4::Scale(const Vec3& s) {
    return Mat4x4(
        s.x, 0, 0, 0,
        0, s.y, 0, 0,
        0, 0, s.z, 0,
        0, 0, 0, 1);
}

Mat4x4 Mat4x4::FromTRS(const Vec3& t, const Quat& r, const Vec3& s) {
	return Translation(t) * r.matrix() * Scale(s);
}

CE_END_NAMESPACE