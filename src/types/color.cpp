#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Color::Color()
        : glm::vec4(0), h(0), s(0), v(0) {}

Color::Color(const float r, const float g, const float b, const float a)
        : glm::vec4(r, g, b, a), h(0), s(0), v(0) {}

Color::Color(const float c, const float a)
        : glm::vec4(c, c, c, a), h(0), s(0), v(0) {}
Color::Color(const glm::vec4& v)
        : glm::vec4(v), h(0), s(0), v(0) {}

void Color::ComputeHSV() {

	float mn = std::min(std::min(r, g), b);
	float mx = std::max(std::max(r, g), b);

	v = mx;
	if (mx > 0) {
		s = (mx - mn) / mx;
		if (mn != mx) {
			if (r == mx) {
				h = (g - b) / (mx - mn);
			}
			else if (g == mx) {
				h = 2.f + (b - r) / (mx - mn);
			}
			else {
				h = 4.f + (r - g) / (mx - mn);
			}
			h /= 6;
			if (h < 0) h += 1;
		}
	}
}

void Color::ComputeRGB() {
	Vec4 ct;
	ct.r = Math::Clamp(std::abs(h * 6 - 3) - 1.f, 0.f, 1.f);
	ct.g = 1 - Math::Clamp(std::abs(h * 6 - 2) - 1.f, 0.f, 1.f);
	ct.b = 1 - Math::Clamp(std::abs(h * 6 - 4) - 1.f, 0.f, 1.f);
	ct.a = 1;
	*((Vec4*)this) = Math::Lerp(Math::Lerp(ct, Vec4(1), 1 - s), Vec4(0), 1 - v);
}

Color Color::FromHSV(const float h, const float s, const float v, const float a) {
	Color c;
	c.h = h;
	c.s = s;
	c.v = v;
	c.ComputeRGB();
	c.a = a;
	return c;
}

Color Color::black() {
    return Color(0);
}
Color Color::white() {
    return Color(1);
}
Color Color::red() {
    return Color(1, 0, 0);
}
Color Color::green() {
    return Color(0, 1, 0);
}
Color Color::blue() {
    return Color(0, 0, 1);
}
Color Color::yellow() {
    return Color(1, 1, 0);
}
Color Color::cyan() {
    return Color(0, 1, 1);
}
Color Color::gray() {
	return Color(0.5f);
}

CE_END_NAMESPACE