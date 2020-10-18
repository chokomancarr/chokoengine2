#include "backend/chokoengine_backend.hpp"

CE_BEGIN_NAMESPACE

namespace {
	void DoTree(std::string& s, const std::vector<SceneObject>& objs, const std::vector<bool>& level) {
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

	SceneObject DoFindByName(const std::vector<SceneObject>& oo, const std::string& nm) {
		for (auto& o : oo) {
			if (o->name() == nm) return o;
			const auto& oc = DoFindByName(o->children(), nm);
			if (!!oc) return oc;
		}
		return nullptr;
	}
}

_Scene::_Scene() : _objects(0), _sky(nullptr), _callbackFlags(0) {}

SceneObject _Scene::AddNewObject(const SceneObject& parent) {
	auto o = SceneObject::New("New Object");
	if (!parent) {
		_objects.push_back(o);
		o->_scene = get_shared<_Scene>();
	}
	else {
		o->parent(parent);
		o->_scene = parent->_scene;
	}
	return o;
}

void _Scene::AddObject(const SceneObject& o, const SceneObject& parent) {
	o->_scene = get_shared<_Scene>();
	if (!parent) _objects.push_back(o);
	else o->parent(parent);
}

void _Scene::RemoveObject(const SceneObject& o) {
	auto& vec = (!o->parent())? _objects : o->_parent->_children;
	auto oo = std::find_if(vec.begin(), vec.end(), [&](const SceneObject& p) {
		return p == o;
	});

	if (oo == vec.end()) {
		Debug::Warning("Scene", "Cannot remove object: object is not part of the scene!");
	}
	(*oo)->_scene = pScene();
	vec.erase(oo);
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

#define TRYCALL(fn)\
	if ((c->_callbackMask & _callbackFlags) == _callbackFlags){\
		c->fn();\
	}

#define RECCALL(nm, fn)\
static const std::function<void(const std::vector<SceneObject>&)> nm\
		= [this](const std::vector<SceneObject>& oo) {\
	for (auto& o : oo) {\
		fn\
		nm(o->_children);\
	}\
}

void _Scene::Update() {
	RECCALL(DoUpdate,
		if (!o->_wasActive) {
			for (auto& c : o->_components) {
				TRYCALL(OnStart);
			}
			o->_wasActive = true;
		}
		for (auto& c : o->_components) {
			TRYCALL(OnUpdate);
		});

	DoUpdate(_objects);
}

#define IMPLCB(nm)\
void _Scene::nm() {\
	RECCALL(Do,\
		for (auto& c : o->_components) {\
			TRYCALL(On ## nm);\
		});\
	Do(_objects);\
}

IMPLCB(LateUpdate)
IMPLCB(PhysicsUpdate)
IMPLCB(PostLogic)
IMPLCB(Paint)

void _Scene::Render() {
	Backend::Renderer::Render(get_shared<_Scene>());
}

void _Scene::PrepareSceneForRendering() {
	Backend::Renderer::RegisterScene(get_shared<_Scene>());
	Backend::Renderer::RenderAuxiliary();
}

std::vector<Camera> _Scene::GetActiveCameras() {
	return Backend::Renderer::cameras;
}

void _Scene::RenderCameras(const std::vector<Camera>& cs) {
	for (auto& c : cs) {
		Backend::Renderer::RenderCamera(c);
	}
}

std::string _Scene::Tree() {
	std::string result = "Scene Tree\n";
	DoTree(result, _objects, std::vector<bool>());
	return result;
}

CE_END_NAMESPACE
