#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _SceneObject : public Object { CE_OBJECT_COMMON
	std::vector<Component> _components;

	std::vector<SceneObject> _children;
	SceneObject _parent;

public:
	_SceneObject(const std::string& nm = "", 
			const Vec3& pos = Vec3(), 
			const Quat& rot = Quat(1, 0, 0, 0),
			const Vec3& scl = Vec3(1));

	CE_GET_MEMBER(children);

	CE_GET_MEMBER(parent);
	
	void parent(const SceneObject& p);

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

	friend class _Scene;
};

CE_END_NAMESPACE

#include "detail/sceneobject.inl"