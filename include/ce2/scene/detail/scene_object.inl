#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template<class T, class... Args>
T _SceneObject::AddComponent(Args&&... args) {
	static_assert(std::is_base_of<_Component, typename T::_TpBase>::value, "AddComponent must be called with component type!");
	auto nt = T::New(std::forward<Args>(args)...);
	_components.push_back(static_cast<Component>(nt));
	nt->_object = std::static_pointer_cast<_SceneObject>(shared_from_this());
	return nt;
}

template<class T>
T _SceneObject::GetComponent() const {
	static_assert(std::is_base_of<_Component, typename T::_TpBase>::value, "GetComponent must be called with component type!");
	for (auto& c : _components) {
		auto res = std::dynamic_pointer_cast<typename T::_TpBase>(c.object());
		if (res) return static_cast<T>(c);
	}
	return T();
}

CE_END_NAMESPACE