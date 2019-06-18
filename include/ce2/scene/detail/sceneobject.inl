#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template<class T>
T _SceneObject::AddComponent() {
	auto nt = T::New();
	_components.push_back(static_cast<Component>(nt));
	nt->_object = std::static_pointer_cast<_SceneObject>(shared_from_this());
	return nt;
}

template<class T>
T _SceneObject::GetComponent() {
	for (auto& c : _components) {
		auto res = std::dynamic_pointer_cast<typename T::_TpBase>(c.object());
		if (res) return static_cast<T>(c);
	}
	return T();
}

CE_END_NAMESPACE