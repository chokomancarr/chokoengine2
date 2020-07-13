#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Int3 {
public:
	Int3(int v = 0);
	Int3(int x, int y, int z);
	Int3(Int2 v, int z);

	int x, y, z;

	explicit operator Int2() const;
	operator Vec3() const;

	int& operator [](const int i);

	Int3 operator -() const;

	Int3 operator +(const Int3& rhs) const;
	Int3 operator -(const Int3& rhs) const;
	Int3 operator *(const Int3& rhs) const;
	Int3 operator /(const Int3& rhs) const;

	bool operator ==(const Int3& rhs) const;
	bool operator !=(const Int3& rhs) const;

	Int3& operator +=(const Int3& rhs);
	Int3& operator -=(const Int3& rhs);
	Int3& operator *=(const Int3& rhs);
	Int3& operator /=(const Int3& rhs);

	std::string str() const;
};

CE_END_NAMESPACE