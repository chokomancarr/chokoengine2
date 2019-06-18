#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Rect::Rect()
        : _x(0), _y(0), _w(0), _h(0) {}

Rect::Rect(float x, float y, float w, float h)
        : _x(x), _y(y), _w(w), _h(h) {}

float Rect::area() const {
    return _w * _w + _h * _h;
}

float Rect::x2() const {
    return _x + _w;
}

float Rect::y2() const {
    return _y + _h;
}

bool Rect::Contains(const Vec2& v) const {
    return (
        (v.x > _x) && (v.x < x2()) &&
        (v.y > _y) && (v.y < y2())
    );
}

bool Rect::Contains(const Rect& r) const {
    return (
        (r.x() > _x) && (r.x2() < x2()) &&
        (r.y() > _y) && (r.y2() < y2())
    );
}

bool Rect::Intersects(const Rect& r) const {
    return (
        Contains(Vec2(r.x(), r.y())) ||
        Contains(Vec2(r.x2(), r.y())) ||
        Contains(Vec2(r.x(), r.y2())) ||
        Contains(Vec2(r.x2(), r.y2()))
        ||
        r.Contains(Vec2(_x, _y)) ||
        r.Contains(Vec2(x2(), _y)) ||
        r.Contains(Vec2(_x, y2())) ||
        r.Contains(Vec2(x2(), y2()))
    );
}

CE_END_NAMESPACE