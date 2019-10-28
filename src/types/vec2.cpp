#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Vec2::Vec2() : glm::vec2() {}

Vec2::Vec2(float w) : glm::vec2(w) {}

Vec2::Vec2(float x, float y) : glm::vec2(x, y) {}

Vec2::Vec2(const glm::vec2& q) : glm::vec2(q) {}

Vec2 Vec2::operator *(const Vec2& rhs) {
	return Vec2(x * rhs.x, y * rhs.y);
}

Vec2 Vec2::operator +(const Vec2& rhs) {
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator -(const Vec2& rhs) {
	return Vec2(x - rhs.x, y - rhs.y);
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

Vec2 Vec2::normalized() {
	return static_cast<Vec2>(glm::normalize((glm::vec2)*this));
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