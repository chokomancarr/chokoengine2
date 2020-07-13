#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Vec4::Vec4(float v) : x(v), y(v), z(v), w(v) {}

Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Vec4::Vec4(Vec2 a, Vec2 b) : x(a.x), y(a.y), z(b.y), w(b.y) {}

Vec4::Vec4(Vec3 v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

Vec4::operator Vec2() const {
	return Vec2(x, y);
}

Vec4::operator Vec3() const {
	return Vec3(x, y, z);
}

float& Vec4::operator [](const int i) {
	return (&x)[i];
}

Vec4 Vec4::operator -() const {
	return Vec4(-x, -y, -z, -w);
}

Vec4 Vec4::operator +(const Vec4& rhs) const {
	return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

Vec4 Vec4::operator -(const Vec4& rhs) const {
	return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

Vec4 Vec4::operator *(const Vec4& rhs) const {
	return Vec4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
}

Vec4 Vec4::operator /(const Vec4& rhs) const {
	return Vec4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
}

bool Vec4::operator ==(const Vec4& rhs) const {
	return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w);
}

bool Vec4::operator !=(const Vec4& rhs) const {
	return (x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w);
}

Vec4& Vec4::operator +=(const Vec4& rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}

Vec4& Vec4::operator -=(const Vec4& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}

Vec4& Vec4::operator *=(const Vec4& rhs) {
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	w *= rhs.w;
	return *this;
}

Vec4& Vec4::operator /=(const Vec4& rhs) {
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	w /= rhs.w;
	return *this;
}

float Vec4::dot(const Vec4& v) const {
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

Vec4 Vec4::normalized() const {
	return (*this) / length();
}

float Vec4::length() const {
	return std::sqrt(x * x + y * y + z * z + w * w);
}

float Vec4::length2() const {
	return x * x + y * y + z * z + w * w;
}

std::string Vec4::str() const {
	return "Vector(" + std::to_string(x) + ", "
		+ std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
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