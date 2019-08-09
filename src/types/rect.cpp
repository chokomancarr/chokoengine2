#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Rect::Rect()
        : _x(0), _y(0), _w(0), _h(0) {}

Rect::Rect(float x, float y, float w, float h)
        : _x(x), _y(y), _w(w), _h(h) {}

float Rect::x2() const {
    return _x + _w;
}

float Rect::y2() const {
    return _y + _h;
}

Rect Rect::sub(float dx1, float dy1, float dx2, float dy2) const {
    return Rect(_x + dx1, _y + dy1, _w - dx1 - dx2, _h - dy1 - dy2);
}

float Rect::area() const {
	return _w * _w + _h * _h;
}

Rect Rect::operator +(const Rect& rhs) const {
	return Rect(_x + rhs._x, _y + rhs._y, _w + rhs._w, _h + rhs._h);
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

Rect Rect::Intersection(const Rect& r) const {
	const float ox = std::max(_x, r._x);
	const float oy = std::max(_y, r._y);
	const float p2x = std::min(x2(), r.x2());
	const float p2y = std::min(y2(), r.y2());
	return Rect(ox, oy, p2x - ox, p2y - oy);
}

CE_END_NAMESPACE