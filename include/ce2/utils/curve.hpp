#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <typename T>
class Curve {
public:
    typedef std::pair<float, T> _KeyTp;

private:
    std::vector<_KeyTp> _keys;

public:
    Curve();
    Curve(const std::vector<T>& keys, float start, float end);
    
    void AddKey(float t, const T& k);

	CE_GET_SET_MEMBER(keys);

    T Eval(float t) const;
};

CE_END_NAMESPACE

#include "detail/curve.inl"