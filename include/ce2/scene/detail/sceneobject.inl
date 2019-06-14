#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template<typename T>
T _SceneObject::GetComponent() {
	for (auto& c : _components) {
		auto res = dynamic_cast<T*>(&c);
		if (res) return *res;
	}
	return T();
}

CE_END_NAMESPACE