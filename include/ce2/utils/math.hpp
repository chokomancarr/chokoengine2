#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Math {
public:
    template <typename T, typename F>
    static T Lerp(const T& a, const T& b, const F& t);

    template <typename T>
    static T ILerp(const T& a, const T& b, const T& c);
};

CE_END_NAMESPACE

#include "detail/math.inl"