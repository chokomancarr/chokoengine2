#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <typename T>
Curve<T>::Curve() : _keys({}) {}

template <typename T>
Curve<T>::Curve(const std::vector<T>& k, float start, float end) {
    const auto sz = k.size();
	_keys.resize(sz);
    for (size_t a = 0; a < sz; a++) {
		_keys[a] = std::make_pair(Math::Lerp(start, end, a / (sz - 1.f)), k[a]);
    }
}

template <typename T>
void Curve<T>::AddKey(float t, const T& k) {
    const auto it = std::find_if(_keys.begin(), _keys.end(), [&](const std::pair<float, T>& v) {
        return v.first > t;
    });
	_keys.insert(it, std::make_pair(t, k));
}

template <typename T>
T Curve<T>::Eval(float t) const {
	if (_keys[0].first >= t) {
		return _keys[0].second;
	}
	for (auto& k : _keys) {
		if (k.first > t) {
			const auto& kb = (&k)[-1];
			return Math::Lerp(kb.second, k.second, Math::ILerp(kb.first, k.first, t));
		}
	}
	return _keys.back().second;
}

CE_END_NAMESPACE