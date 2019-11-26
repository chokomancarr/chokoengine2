#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_SceneObject::_SceneObject(const std::string& nm, 
        const Vec3& pos, const Quat& rot, const Vec3& scl) : _Object() {
    _name = nm;
	_transform._object = this;
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
        tp = p->_parent;
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
    }

    /* Add to new parent
     */
    _parent = p;
    p->_children.push_back(get_shared<_SceneObject>());

    _transform.UpdateParentMatrix();
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