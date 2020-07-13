#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Color {
public:
    Color();
    Color(const float r, const float g, const float b, const float a = 1);
    Color(const float c, const float a = 1);
	Color(const Vec4& v);

	operator Vec4() const;

	float r, g, b, a;
	float h, s, v;

	Color operator *(const Color& c) const;
	Color operator *(const float f) const;

	Color operator +(const Color& c) const;
	Color operator -(const Color& c) const;

	void ComputeHSV();
	void ComputeRGB();

	static Color FromHSV(const float h, const float s, const float v, const float a = 1);

    static Color black();
    static Color white();
    static Color red();
    static Color green();
    static Color blue();
    static Color yellow();
    static Color cyan();
	static Color gray();
};

CE_END_NAMESPACE