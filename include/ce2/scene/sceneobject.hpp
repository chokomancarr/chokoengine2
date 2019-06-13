#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

class _SceneObject { CE_OBJECT_COMMON

	std::vector<Component> _components;

public:

	/* Returns component of type T attached to this object
	 */
	template<typename T>
	T GetComponent();

};

END_CE_NAMESPACE

#include "detail/sceneobject.inl"