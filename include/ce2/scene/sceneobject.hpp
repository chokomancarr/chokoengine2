#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _SceneObject { CE_OBJECT_COMMON

	std::vector<Component> _components;

public:

	/* Adds a new component to the object
	 */
	template<typename T>
	T AddComponent();

	/* Retrieves a component in the object
	 */
	template <typename T>
	T GetComponent();

	/* Removes a component from the object
	 */
	void RemoveComponent(const Component& c);
};

CE_END_NAMESPACE

#include "detail/sceneobject.inl"