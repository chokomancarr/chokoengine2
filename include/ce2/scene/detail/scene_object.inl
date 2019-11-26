#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template<class T, class... Args>
typename std::enable_if<std::is_base_of<_Component, typename T::_TpBase>::value, T>::type
 _SceneObject::AddComponent(Args&&... args) {
	auto nt = T::New(std::forward<Args>(args)...);
	_components.push_back(nt);
	nt->_object = std::static_pointer_cast<_SceneObject>(shared_from_this());
	return nt;
}

template<class T>
typename std::enable_if<std::is_base_of<_Component, typename T::_TpBase>::value, T>::type
_SceneObject::GetComponent() const {
	for (auto& c : _components) {
		auto res = std::dynamic_pointer_cast<typename T::_TpBase>(c.object());
		if (res) return static_cast<T>(c);
	}
	return T();
}

CE_END_NAMESPACE
