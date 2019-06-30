#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _SceneObject : public Object { CE_OBJECT_COMMON
	Transform _transform;

	std::vector<Component> _components;

	std::vector<SceneObject> _children;
	SceneObject _parent;

public:
	_SceneObject(const std::string& nm = "", 
			const Vec3& pos = Vec3(),
			const Quat& rot = Quat(1, 0, 0, 0),
			const Vec3& scl = Vec3(1));

	pTransform transform();

	CE_GET_MEMBER(components);
	CE_GET_MEMBER(children);

	CE_GET_MEMBER(parent);
	
	void parent(const SceneObject& p);

	/* Adds a new component to the object
	 */
	template<class T, class... Args>
	T AddComponent(Args&&... args);

	/* Retrieves a component in the object
	 */
	template <class T>
	T GetComponent();

	/* Removes a component from the object
	 */
	void RemoveComponent(const Component& c);

	friend class _Scene;
};

CE_END_NAMESPACE

#include "detail/scene_object.inl"