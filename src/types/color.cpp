#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Color::Color()
        : glm::vec4(0) {}

Color::Color(const float r, const float g, const float b, const float a)
        : glm::vec4(r, g, b, a) {}

Color::Color(const float c, const float a)
        : glm::vec4(c, c, c, a) {}
Color::Color(const glm::vec4& v)
        : glm::vec4(v) {}

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
Color Color::gray(float f) {
	return Color(f, f, f);
}

CE_END_NAMESPACE