#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

template<typename T>
T& _SceneObject::GetComponent() {
	for (auto& c : _components) {
		auto xx = std::dynamic_cast<T>(c);
		if (xx != nullptr) {
			return xx;
		}
	}
}

END_CE_NAMESPACE