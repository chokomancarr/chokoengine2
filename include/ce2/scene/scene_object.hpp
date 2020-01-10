#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* Objects in a scene
 */
class _SceneObject : public _Object { CE_OBJECT_COMMON
public:
	/* Information on which prefab spawned this object
	 */
	class PrefabInfo {
	public:
		bool hasInfo;

		pSceneObject head; //top object spawned, null for this object
		/* the values below are only read from head
		 */
		pPrefab prefab; //closest prefab spawner
		/* list of objects spawned, including those from sub-prefabs
		 * this determines which objects to exclude during serialization
		 */
		std::vector<ChokoEngine::objectid> ids;

		PrefabInfo() : hasInfo(false), head(nullptr), prefab(nullptr), ids({}) {}

		bool operator !() const {
			return !hasInfo;
		}
	};

private:
	Transform _transform;

	std::vector<Component> _components;

	std::vector<SceneObject> _children;
	
	pScene _scene;
	pSceneObject _parent;

	PrefabInfo _prefabInfo;

	bool _wasActive;

	void UnlinkPrefabTree();

public:
	_SceneObject(const std::string& nm = "", 
			const Vec3& pos = Vec3(),
			const Quat& rot = Quat(1, 0, 0, 0),
			const Vec3& scl = Vec3(1));

	_SceneObject(const _SceneObject&) = delete;

	/* Transformation data of the object
	 */
	pTransform transform();

	/* List of components attached to the object
	 */
	CE_GET_MEMBER(components);

	/* List of sub-objects attached to the object
	 */
	CE_GET_MEMBER(children);

	/* Parent object this object is attached to
	 */
	CE_GET_MEMBER(parent);
	void parent(const SceneObject& p);

	/* Prefab infomation this object is spawned from
	 */
	CE_GET_MEMBER(prefabInfo);

	/* Scene this object is attached to
	 */
	CE_GET_MEMBER(scene);

	/* Adds a new component to the object
	 */
	template<class T, class... Args>
	typename std::enable_if<std::is_base_of<_Component,
		typename T::_TpBase>::value, T>::type
	AddComponent(Args&&... args);

	/* Retrieves a component in the object
	 */
	template <class T>
	typename std::enable_if<std::is_base_of<_Component,
		typename T::_TpBase>::value, T>::type
	GetComponent() const;

	/* Removes a component from the object
	 */
	void RemoveComponent(const Component& c);

	/* Returns a copy of the object tree
	 * All children objects and components will also be copied
	 * However, object references in components will not change
	 */
	SceneObject Clone() const;

	void Delete() override;

	friend class _Scene;
	friend class _PrefabComp;
};

CE_END_NAMESPACE

#include "detail/scene_object.inl"
