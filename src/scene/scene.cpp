#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

void _Scene::DoTree(std::string& s, const std::vector<SceneObject>& objs, int level) {
    auto sz = objs.size();
    if (!sz) return;
    for (size_t a = 0; a < sz; a++) {
        for (int b = 0; b < level; b++) {
            s += "|  ";
        }
        s += (a == sz - 1) ? "`--" : "+--";
        s += objs[a]->name() + "\n";
        DoTree(s, objs[a]->children(), level + 1);
    }
}

_Scene::_Scene(const std::string& nm) : Object(nm), _objects({}) {}

SceneObject _Scene::AddNewObject(const SceneObject& parent) {
    auto& vec = (!parent)? _objects : parent->_children;
    vec.push_back(SceneObject::New());
    return vec.back();
}

void _Scene::AddObject(const SceneObject& o, const SceneObject& parent) {
    auto& vec = (!parent)? _objects : parent->_children;
    for (auto& oo : vec) {
        if (oo == o) {
            Debug::Warning("Scene", "Cannot add object: object already exists in scene!");
            return;
        }
    }
    _objects.push_back(o);
}

void _Scene::RemoveObject(const SceneObject& o) {
    auto& vec = (!o->parent())? _objects : o->_parent->_children;
    auto oo = std::find_if(vec.begin(), vec.end(), [&](const SceneObject& p) {
        return p == o;
    });

    if (oo == vec.end()) {
        Debug::Warning("Scene", "Cannot remove object: object is not a part of the scene!");
    }
    std::swap(*oo, vec.back());
    _objects.pop_back();
}

std::string _Scene::Tree() const {
    std::string result = _name + "\n";
    DoTree(result, _objects, 0);
    return result;
}

CE_END_NAMESPACE