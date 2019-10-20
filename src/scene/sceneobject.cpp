#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_SceneObject::_SceneObject(const std::string& nm, 
        const Vec3& pos, const Quat& rot, const Vec3& scl) : _Asset() {
    _name = nm;
	_transform._object = this;
}

_SceneObject::~_SceneObject() {
	Delete();
}

pTransform _SceneObject::transform() {
    return &_transform;
}

void _SceneObject::parent(const SceneObject& p) {
    if (p == _parent) return;

    /* Checks for circular parenting
     */
    if (p.operator->() == this) {
        Debug::Warning("SceneObject", "Cannot set object parent: parent is self!");
        return;
    }
    SceneObject tp = p->_parent;
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
    p->_children.push_back(Ref<_SceneObject>(std::static_pointer_cast<_SceneObject>(shared_from_this())));

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