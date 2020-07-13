#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Int4 {
public:
	Int4(int v = 0);
	Int4(int x, int y, int z, int w);
	Int4(Int2 a, Int2 b);
	Int4(Int3 v, int w);

	int x, y, z, w;

	explicit operator Int2() const;
	explicit operator Int3() const;
	operator Vec4() const;

	int& operator [](const int i);

	Int4 operator -() const;

	Int4 operator +(const Int4& rhs) const;
	Int4 operator -(const Int4& rhs) const;
	Int4 operator *(const Int4& rhs) const;
	Int4 operator /(const Int4& rhs) const;

	bool operator ==(const Int4& rhs) const;
	bool operator !=(const Int4& rhs) const;

	Int4& operator +=(const Int4& rhs);
	Int4& operator -=(const Int4& rhs);
	Int4& operator *=(const Int4& rhs);
	Int4& operator /=(const Int4& rhs);

	std::string str() const;
};

CE_END_NAMESPACE