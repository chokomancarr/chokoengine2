#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Vec2::Vec2(float w) : x(w), y(w) {}

Vec2::Vec2(float x, float y) : x(x), y(y) {}

float& Vec2::operator [](const int i) {
	return (&x)[i];
}

Vec2 Vec2::operator-() const {
	return Vec2(-x, -y);
}

Vec2 Vec2::operator +(const Vec2& rhs) const {
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator -(const Vec2& rhs) const {
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator *(const Vec2& rhs) const {
	return Vec2(x * rhs.x, y * rhs.y);
}

Vec2 Vec2::operator /(const Vec2& rhs) const {
	return Vec2(x / rhs.x, y / rhs.y);
}

bool Vec2::operator ==(const Vec2& rhs) const {
	return x == rhs.x && y == rhs.y;
}

bool Vec2::operator !=(const Vec2& rhs) const {
	return (x != rhs.x) || (y != rhs.y);
}

Vec2& Vec2::operator +=(const Vec2& rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Vec2& Vec2::operator -=(const Vec2& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

Vec2& Vec2::operator *=(const Vec2& rhs) {
	x *= rhs.x;
	y *= rhs.y;
	return *this;
}

Vec2& Vec2::operator /=(const Vec2& rhs) {
	x /= rhs.x;
	y /= rhs.y;
	return *this;
}

Vec2 Vec2::normalized() const {
	return (*this) / length();
}

float Vec2::length() const {
	return std::sqrt(length2());
}

float Vec2::length2() const {
	return x * x + y * y;
}

float Vec2::dot(const Vec2& rhs) const {
	return x * rhs.x + y * rhs.y;
}

std::string Vec2::str() const {
	return "Vector(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

Vec2 Vec2::left() {
	return Vec2(-1, 0);
}

Vec2 Vec2::right() {
	return Vec2(1, 0);
}

Vec2 Vec2::up() {
	return Vec2(0, 1);
}

Vec2 Vec2::down() {
	return Vec2(0, -1);
}

CE_END_NAMESPACE