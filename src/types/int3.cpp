#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Int3::Int3(int w) : x(w), y(w), z(w) {}

Int3::Int3(int x, int y, int z) : x(x), y(y), z(z) {}

Int3::Int3(Int2 v, int z) : x(v.x), y(v.y) {}

Int3::operator Int2() const {
	return Int2(x, y);
}

Int3::operator Vec3() const {
	return Vec3(x, y, z);
}

int& Int3::operator [](const int i) {
	return (&x)[i];
}

Int3 Int3::operator -() const {
	return Int3(-x, -y, -z);
}

Int3 Int3::operator +(const Int3& rhs) const {
	return Int3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Int3 Int3::operator -(const Int3& rhs) const {
	return Int3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Int3 Int3::operator *(const Int3& rhs) const {
	return Int3(x * rhs.x, y * rhs.y, z * rhs.z);
}

Int3 Int3::operator /(const Int3& rhs) const {
	return Int3(x / rhs.x, y / rhs.y, z / rhs.z);
}

bool Int3::operator ==(const Int3& rhs) const {
	return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
}

bool Int3::operator !=(const Int3& rhs) const {
	return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
}

Int3& Int3::operator +=(const Int3& rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

Int3& Int3::operator -=(const Int3& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

Int3& Int3::operator *=(const Int3& rhs) {
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	return *this;
}

Int3& Int3::operator /=(const Int3& rhs) {
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	return *this;
}

std::string Int3::str() const {
	return "iVector(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
}

CE_END_NAMESPACE