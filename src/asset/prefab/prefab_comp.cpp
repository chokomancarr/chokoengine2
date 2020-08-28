#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_PrefabComp::_PrefabComp(const Component& c)
		: type(c->componentType) {
	name = ComponentTypeStr.at(type);
#define CS(tp) case ComponentType::tp: Set ## tp((tp)c); break;
	switch (type) {
		CS(Camera)
		CS(Collider)
		CS(Animator)
		CS(Light)
		CS(MeshRenderer)
		CS(Rig)
		CS(Rigidbody)
		CS(Script)
	default:
		CE_NOT_IMPLEMENTED
	}
#undef CS
}

_PrefabComp::_PrefabComp(const JsonPair& json) : _PrefabObjBase(json.value) {
	auto it = std::find_if(ComponentTypeStr.begin(), ComponentTypeStr.end(), 
		[&json](decltype(ComponentTypeStr)::const_reference i) {
			return i.second == json.key.string;
	});
	if(it != ComponentTypeStr.end()) {
		type = it->first;
	}
	else {
		Debug::Warning("PrefabComponent", "Unknown type: \"" + json.key.string + "\"!");
	}
}

SceneObject _PrefabComp::Instantiate(const SceneObject& o) const {
#define CS(tp) case ComponentType::tp: Instantiate ## tp(o); break;
	switch (type) {
		CS(Camera)
		CS(Collider)
		CS(Animator)
		CS(Light)
		CS(MeshRenderer)
		CS(Rig)
		CS(Rigidbody)
		CS(Script)
	default:
		CE_NOT_IMPLEMENTED
	}

	return nullptr;
}

CE_END_NAMESPACE