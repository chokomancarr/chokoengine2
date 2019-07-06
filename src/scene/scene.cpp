#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

std::vector<SceneObject> Scene::_objects;

Background Scene::_sky;

void Scene::DoTree(std::string& s, const std::vector<SceneObject>& objs, const std::vector<bool>& level) {
    auto lvl2 = level;
    lvl2.push_back(true);
    auto sz = objs.size();
    if (!sz) return;
    for (size_t a = 0; a < sz; a++) {
        for (auto l : level) {
            s += l ? "|  " : "   ";
        }
        s += (a == sz - 1) ? "`--" : "+--";
        s += objs[a]->name() + " [";
        for (auto& c : objs[a]->components()) {
            s += " " + c->name() + ",";
        }
        if (s.back() != '[') {
            s.back() = ' ';
        }
        s += "]\n";
        if (a == sz - 1)
            lvl2.back() = false;
        DoTree(s, objs[a]->children(), lvl2);
    }
}

SceneObject Scene::AddNewObject(const SceneObject& parent) {
    auto& vec = (!parent)? _objects : parent->_children;
    auto o = SceneObject::New();
    vec.push_back(o);
    o->_parent = parent;
    return o;
}

void Scene::AddObject(const SceneObject& o, const SceneObject& parent) {
    auto& vec = (!parent)? _objects : parent->_children;
    for (auto& oo : vec) {
        if (oo == o) {
            Debug::Warning("Scene", "Cannot add object: object already exists in scene!");
            return;
        }
    }
    _objects.push_back(o);
    o->_parent = parent;
}

void Scene::RemoveObject(const SceneObject& o) {
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

std::string Scene::Tree() {
    std::string result = "Scene Tree\n";
    DoTree(result, _objects, std::vector<bool>());
    return result;
}

void Scene::ClearObjects() {
    _objects.clear();
}

CE_END_NAMESPACE