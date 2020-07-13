#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Int2 {
public:
	Int2(const int v = 0);
	Int2(const int x, int y);

	int x, y;

	operator Vec2() const;

	int& operator [](const int i);

	Int2 operator -() const;

	Int2 operator +(const Int2& rhs) const;
	Int2 operator -(const Int2& rhs) const;
	Int2 operator *(const Int2& rhs) const;
	Int2 operator /(const Int2& rhs) const;

	bool operator ==(const Int2& rhs) const;
	bool operator !=(const Int2& rhs) const;

	Int2& operator +=(const Int2& rhs);
	Int2& operator -=(const Int2& rhs);
	Int2& operator *=(const Int2& rhs);
	Int2& operator /=(const Int2& rhs);

	std::string str() const;
};

CE_END_NAMESPACE