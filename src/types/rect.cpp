#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

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

END_CE_NAMESPACE