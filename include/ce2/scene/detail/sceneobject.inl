#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template<typename T>
T _SceneObject::AddComponent() {
	_components.push_back(static_cast<Component>(T::New()));
	return static_cast<T>(_components.back());
}

template<typename T>
T _SceneObject::GetComponent() {
	for (auto& c : _components) {
		auto res = dynamic_cast<T*>(&c);
		if (res) return static_cast<T>(*res);
	}
	return T();
}

CE_END_NAMESPACE