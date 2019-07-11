#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <typename T>
Curve::Curve() : keys({}) {}

template <typename T>
Curve::Curve(const std::vector<T>& k, float start, float end) {
    const auto sz = k.size();
    keys.resize(sz);
    for (size_t a = 0; a < sz; a++) {
        keys.push_back(std::make_pair(Math::Lerp(start, end, a / (sz - 1.f)), k[a]));
    }
}

template <typename T>
void Curve::AddKey(float t, const T& k) {
    const auto& it = std::find_if(keys.begin(), keys.end(), [&](const T& v) {
        return v.first > t;
    });
    std::insert(it, k);
}

template <typename T>
T Eval(float t) {
    const auto& it = std::lower_bound(keys.begin(), keys.end(), _KeyTp(t, T()), [&](const _KeyTp& a, const _KeyTp& b) {
        return a.first < b.first;
    })
    if (it == keys.begin()) return it->second;
    return Math::Lerp((it-1)->second, it->second, Math::ILerp((it-1)->first, it->first, t));
}

CE_END_NAMESPACE