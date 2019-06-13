#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

class Rect { CE_OBJECT_COMMON
    float _x, _y, _w, _h;

public:
    Rect();
    Rect(float x, float y, float w, float h);

    GET_SET_MEMBER_FUNC(float, x);
    GET_SET_MEMBER_FUNC(float, y);
    GET_SET_MEMBER_FUNC(float, w);
    GET_SET_MEMBER_FUNC(float, h);
    float x2() const;
    float y2() const;

    float area() const;
};

END_CE_NAMESPACE