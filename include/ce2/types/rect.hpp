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
};

CE_END_NAMESPACE