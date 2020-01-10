#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

void _SceneObject::UnlinkPrefabTree() {
	if (!_prefabInfo.prefab) return;
	if (!_prefabInfo.head) { //this object is the head

	}
	else {

	}
}

_SceneObject::_SceneObject(const std::string& nm, const Vec3& pos, const Quat& rot,
		const Vec3& scl) : _Object(nm), _components({}), _children(0), _scene(nullptr),
		_parent(nullptr), _prefabInfo(), _wasActive(false) {
	_transform._object = this;
	_transform._localPosition = pos;
	_transform._localScale = scl;
	_transform.localRotation(rot);
}

pTransform _SceneObject::transform() {
	return &_transform;
}

void _SceneObject::parent(const SceneObject& _p) {
	const pSceneObject p(_p);
	if (p == _parent) return;

	/* Checks for circular parenting
	 */
	if (p.operator->() == this) {
		Debug::Warning("SceneObject", "Cannot set object parent: parent is self!");
		return;
	}
	pSceneObject tp = p->_parent;
	while (!!tp) {
		if (tp.operator->() == this) {
			Debug::Warning("SceneObject", "Cannot set object parent: parent is a child of object!");
			return;
		}
		tp = tp->_parent;
	}

	/* Removes the old parent if it exists
	 */
	if (!!_parent) {
		auto cc = std::find_if(p->_children.begin(), p->_children.end(), [&](const SceneObject& pp) {
			return pp.operator->() == this;
		});
		if (cc == _children.end()) {
			Debug::Warning("SceneObject", "Cannot remove component: component is not attached!");
		}
		std::swap(*cc, _children.back());
		_children.pop_back();

		/* TODO: clear prefabs if this object was a child object defined in a prefab,
		 * where the whole instance should become unrelated
		 */
	}

	/* Add to new parent
	 */
	_parent = p;
	p->_children.push_back(get_shared<_SceneObject>());

	const std::function<void(const SceneObject&)> addp = [&addp, &p](const SceneObject& o) {
		o->_prefabs.insert(o->_prefabs.end(), p->_prefabs.begin(), p->_prefabs.end());
		for (auto& c : o->children()) {
			addp(c);
		}
	};

	_prefabs.insert(_prefabs.end(), p->_prefabs.begin(), p->_prefabs.end());
	for (auto& c : _children) {
		addp(c);
	}

	_transform.UpdateParentMatrix();
}

Prefab _SceneObject::prefab() {
	if (!_prefabs.size() || !_prefabs[0]) return nullptr;
	return _prefabs[0].lock();
}

void _SceneObject::RemoveComponent(const Component& c) {
	auto cc = std::find_if(_components.begin(), _components.end(), [&](const Component& p) {
		return p == c;
	});

	if (cc == _components.end()) {
		Debug::Warning("SceneObject", "Cannot remove component: component is not attached!");
	}
	(*cc)->_deleted = true;
	std::swap(*cc, _components.back());
	_components.pop_back();
}

SceneObject _SceneObject::Clone() const {
	std::function<SceneObject(const SceneObject&, const SceneObject&)> doclone = 
			[&](const SceneObject& o, const SceneObject& p) {
		SceneObject copy = SceneObject::New();
		copy->_CopyInfo(o);
		copy->_transform = o->_transform;
		copy->_transform._object = copy.object().get();
		for (auto& c : o->_components) {
			copy->_components.push_back(c->Clone());
			copy->_components.back()->_object = copy;
		}
		for (auto& o2 : o->_children) {
			copy->_children.push_back(doclone(o2, copy));
		}
		copy->_parent = p;
		return copy;
	};

	return doclone(get_shared<_SceneObject>(), nullptr);
}

void _SceneObject::Delete() {
	_Object::Delete();
	for (auto& c : _components) {
		c->Delete();
	}
	for (auto& o : _children) {
		o->Delete();
	}
}

CE_END_NAMESPACE