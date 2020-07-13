#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Int2::Int2(int w) : x(w), y(w) {}

Int2::Int2(int x, int y) : x(x), y(y) {}

Int2::operator Vec2() const {
	return Vec2(x, y);
}

int& Int2::operator [](const int i) {
	return (&x)[i];
}

Int2 Int2::operator-() const {
	return Int2(-x, -y);
}

Int2 Int2::operator +(const Int2& rhs) const {
	return Int2(x + rhs.x, y + rhs.y);
}

Int2 Int2::operator -(const Int2& rhs) const {
	return Int2(x - rhs.x, y - rhs.y);
}

Int2 Int2::operator *(const Int2& rhs) const {
	return Int2(x * rhs.x, y * rhs.y);
}

Int2 Int2::operator /(const Int2& rhs) const {
	return Int2(x / rhs.x, y / rhs.y);
}

bool Int2::operator ==(const Int2& rhs) const {
	return x == rhs.x && y == rhs.y;
}

bool Int2::operator !=(const Int2& rhs) const {
	return (x != rhs.x) || (y != rhs.y);
}

Int2& Int2::operator +=(const Int2& rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Int2& Int2::operator -=(const Int2& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

Int2& Int2::operator *=(const Int2& rhs) {
	x *= rhs.x;
	y *= rhs.y;
	return *this;
}

Int2& Int2::operator /=(const Int2& rhs) {
	x /= rhs.x;
	y /= rhs.y;
	return *this;
}

std::string Int2::str() const {
	return "iVector(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

CE_END_NAMESPACE