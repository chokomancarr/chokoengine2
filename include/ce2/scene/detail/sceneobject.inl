#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

template<typename T>
T _SceneObject::GetComponent() {
	for (auto& c : _components) {
		auto res = dynamic_cast<T*>(&c);
		if (res) return *res;
	}
	return T();
}

END_CE_NAMESPACE