#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE
Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(float w) : x(w), y(w), z(w) {}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3::Vec3(Vec2 v, float z) : x(v.x), y(v.y) {}

Vec3 Vec3::operator +(const Vec3& rhs) const {
	return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vec3 Vec3::operator -(const Vec3& rhs) const {
	return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vec3 Vec3::operator *(const Vec3& rhs) const {
	return Vec3(x * rhs.x, y * rhs.y, z * rhs.z);
}

Vec3 Vec3::operator /(const Vec3& rhs) const {
	return Vec3(x / rhs.x, y / rhs.y, z / rhs.z);
}

Vec3& Vec3::operator +=(const Vec3& rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

Vec3& Vec3::operator -=(const Vec3& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

Vec3& Vec3::operator *=(const Vec3& rhs) {
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	return *this;
}

Vec3& Vec3::operator /=(const Vec3& rhs) {
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	return *this;
}

float Vec3::dot(const Vec3& v) const {
	return x * v.x + y * v.y + z * v.z;
}

Vec3 Vec3::cross(const Vec3& v) const {
	return Vec3(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y - v.x
	);
}

Vec3 Vec3::normalized() const {
	return (*this) / std::sqrt(x*x + y*y + z*z);
}

std::string Vec3::str() const {
	return "Vector(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
}

Vec3 Vec3::left() {
	return Vec3(-1, 0, 0);
}

Vec3 Vec3::right() {
	return Vec3(1, 0, 0);
}

Vec3 Vec3::up() {
	return Vec3(0, 1, 0);
}

Vec3 Vec3::down() {
	return Vec3(0, -1, 0);
}

Vec3 Vec3::front() {
	return Vec3(0, 0, 1);
}

Vec3 Vec3::back() {
	return Vec3(0, 0, -1);
}

Vec3 operator* (const glm::mat3& m, const Vec3& v) {
	auto t = (m * *(glm::vec3*)&v.x);
	return *(Vec3*)&t;
}

CE_END_NAMESPACE