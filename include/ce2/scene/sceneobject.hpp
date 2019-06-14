#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _SceneObject { CE_OBJECT_COMMON

	std::vector<Component> _components;

public:

	/* Returns component of type T attached to this object
	 */
	template<typename T>
	T GetComponent();

};

CE_END_NAMESPACE

#include "detail/sceneobject.inl"