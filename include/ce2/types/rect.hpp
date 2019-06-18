#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Rect { CE_OBJECT_COMMON
    float _x, _y, _w, _h;

public:
    Rect();
    Rect(float x, float y, float w, float h);

    CE_GET_SET_MEMBER(x);
    CE_GET_SET_MEMBER(y);
    CE_GET_SET_MEMBER(w);
    CE_GET_SET_MEMBER(h);
    float x2() const;
    float y2() const;

    float area() const;

    /* Checks if point \p v is inside this rect
     */
    bool Contains(const Vec2& v) const;

    /* Cehcks if rect \p r is inside this rect
     */
    bool Contains(const Rect& r) const;

    /* Checks if any part of this rect is overlapping with \p r
     */
    bool Intersects(const Rect& r) const;
};

CE_END_NAMESPACE