#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Int4::Int4(int v) : x(v), y(v), z(v), w(v) {}

Int4::Int4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}

Int4::Int4(Int2 a, Int2 b) : x(a.x), y(a.y), z(b.y), w(b.y) {}

Int4::Int4(Int3 v, int w) : x(v.x), y(v.y), z(v.z), w(w) {}

Int4::operator Int2() const {
	return Int2(x, y);
}

Int4::operator Int3() const {
	return Int3(x, y, z);
}

Int4::operator Vec4() const {
	return Vec4(x, y, z, w);
}

int& Int4::operator [](const int i) {
	return (&x)[i];
}

Int4 Int4::operator -() const {
	return Int4(-x, -y, -z, -w);
}

Int4 Int4::operator +(const Int4& rhs) const {
	return Int4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

Int4 Int4::operator -(const Int4& rhs) const {
	return Int4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

Int4 Int4::operator *(const Int4& rhs) const {
	return Int4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
}

Int4 Int4::operator /(const Int4& rhs) const {
	return Int4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
}

bool Int4::operator ==(const Int4& rhs) const {
	return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w);
}

bool Int4::operator !=(const Int4& rhs) const {
	return (x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w);
}

Int4& Int4::operator +=(const Int4& rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}

Int4& Int4::operator -=(const Int4& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}

Int4& Int4::operator *=(const Int4& rhs) {
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	w *= rhs.w;
	return *this;
}

Int4& Int4::operator /=(const Int4& rhs) {
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	w /= rhs.w;
	return *this;
}

std::string Int4::str() const {
	return "iVector(" + std::to_string(x) + ", "
		+ std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
}

CE_END_NAMESPACE