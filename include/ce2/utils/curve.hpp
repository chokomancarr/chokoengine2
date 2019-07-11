#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <typename T>
class Curve {
    typedef std::pair<float, T> _KeyTp;

    std::vector<_KeyTp> keys;

public:
    Curve();
    Curve(const std::vector<T>& keys, float start, float end);
    
    void AddKey(float t, const T& k);

    T Eval(float t);
};

CE_END_NAMESPACE

#include "detail/curve.inl"