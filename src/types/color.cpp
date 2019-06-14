#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

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

Color::Color()
        : _r(0), _g(0), _b(0), _a(0) {}

Color::Color(const float r, const float g, const float b, const float a)
        : _r(r), _g(g), _b(b), _a(a) {}

Color::Color(const float c, const float a)
        : _r(c), _g(c), _b(c), _a(a) {}

CE_END_NAMESPACE