#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Vec4::Vec4() : glm::vec4() {}

Vec4::Vec4(float w) : glm::vec4(w) {}

Vec4::Vec4(float x, float y, float z, float w) : glm::vec4(x, y, z, w) {}

Vec4::Vec4(Vec3 v, float z) : glm::vec4(*(glm::vec3*)&v.x, z) {}

Vec4::Vec4(const glm::vec4& q) : glm::vec4(q) {}

Vec4 Vec4::operator *(const Vec4& rhs) {
	return Vec4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
}

Vec4 Vec4::normalized() {
	return static_cast<Vec4>(glm::normalize((glm::vec4)*this));
}

Vec4 Vec4::left(float w) {
	return Vec4(-1, 0, 0, w);
}

Vec4 Vec4::right(float w) {
	return Vec4(1, 0, 0, w);
}

Vec4 Vec4::up(float w) {
	return Vec4(0, 1, 0, w);
}

Vec4 Vec4::down(float w) {
	return Vec4(0, -1, 0, w);
}

Vec4 Vec4::front(float w) {
	return Vec4(0, 0, 1, w);
}

Vec4 Vec4::back(float w) {
	return Vec4(0, 0, -1, w);
}

CE_END_NAMESPACE