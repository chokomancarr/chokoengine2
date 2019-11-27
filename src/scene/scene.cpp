#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

void _Scene::DoTree(std::string& s, const std::vector<SceneObject>& objs, const std::vector<bool>& level) {
    auto lvl2 = level;
    lvl2.push_back(true);
    auto sz = objs.size();
    if (!sz) return;
    for (size_t a = 0; a < sz; a++) {
		std::string sl = "";
        for (auto l : level) {
            sl += l ? "|  " : "   ";
        }
        sl += (a == sz - 1) ? "`--" : "+--";
		s += IO::ColorOutput(sl, TerminalColor::DarkGrey)
			+ objs[a]->name();
		sl = " [";
        for (auto& c : objs[a]->components()) {
            sl += " " + c->name() + ",";
        }
        if (sl.back() != '[') {
            sl.back() = ' ';
        }
		s += IO::ColorOutput(sl + "]\n", TerminalColor::DarkGrey);
        if (a == sz - 1)
            lvl2.back() = false;
        DoTree(s, objs[a]->children(), lvl2);
    }
}

SceneObject _Scene::DoFindByName(const std::vector<SceneObject>& oo, const std::string& nm) {
    for (auto& o : oo) {
        if (o->_name == nm) return o;
        const auto& oc = DoFindByName(o->_children, nm);
        if (!!oc) return oc;
    }
    return nullptr;
}

void _Scene::DoUpdate(const std::vector<SceneObject>& oo) {
	for (auto& o : oo) {
		for (auto& c : o->_components) {
			c->OnUpdate();
		}
		DoUpdate(o->_children);
	}
}

void _Scene::Update() {
	DoUpdate(_objects);
}

_Scene::_Scene() : _objects(0), _sky(nullptr) {}

SceneObject _Scene::AddNewObject(const SceneObject& parent) {
    auto& vec = (!parent)? _objects : parent->_children;
    auto o = SceneObject::New("New Object");
    vec.push_back(o);
    o->_parent = parent;
	o->_scene = get_shared<_Scene>();
    return o;
}

void _Scene::AddObject(const SceneObject& o, const SceneObject& parent) {
    auto& vec = (!parent)? _objects : parent->_children;
    for (auto& oo : vec) {
        if (oo == o) {
            Debug::Warning("Scene", "Cannot add object: object already exists in scene / parent!");
            return;
        }
    }
    vec.push_back(o);
    o->_parent = parent;
	o->_scene = get_shared<_Scene>();
}

void _Scene::RemoveObject(const SceneObject& o) {
    auto& vec = (!o->parent())? _objects : o->_parent->_children;
    auto oo = std::find_if(vec.begin(), vec.end(), [&](const SceneObject& p) {
        return p == o;
    });

    if (oo == vec.end()) {
        Debug::Warning("Scene", "Cannot remove object: object is not a part of the scene!");
    }
	(*oo)->_scene = pScene();
    std::swap(*oo, vec.back());
    _objects.pop_back();
}

std::string _Scene::Tree() {
    std::string result = "Scene Tree\n";
    DoTree(result, _objects, std::vector<bool>());
    return result;
}

void _Scene::ClearObjects() {
    _objects.clear();
}

SceneObject _Scene::FindByName(const std::string& nm) {
    return DoFindByName(_objects, nm);
}

std::vector<SceneObject> _Scene::FindAllByPred(std::function<bool(const SceneObject&)> cond) {
    std::vector<SceneObject> res = {};
    std::function<void(const std::vector<SceneObject>&, std::vector<SceneObject>&)> _f =
        [&](const std::vector<SceneObject>& oo, std::vector<SceneObject>& rs) {
        for (auto& o : oo) {
            if (cond(o)) {
                rs.push_back(o);
            }
            _f(o->_children, rs);
        }
    };
    _f(_objects, res);
    return res;
}

CE_END_NAMESPACE
