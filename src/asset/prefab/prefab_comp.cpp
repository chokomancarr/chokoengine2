#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_PrefabComp::_PrefabComp(const Component& c)
		: type(c->componentType) {
	name = ComponentTypeStr[(int)type];
#define CS(tp) case ComponentType::tp: Set ## tp((tp)c); break;
	switch (type) {
		CS(Camera)
		CS(Animator)
		CS(Light)
		CS(MeshRenderer)
		CS(Rig)
	default:
		CE_NOT_IMPLEMENTED
	}
#undef CS
}

_PrefabComp::_PrefabComp(const JsonPair& json) : _PrefabObjBase(json.value) {
	CE_GET_IT(it, ComponentTypeStr, json.key.string) {
		type = (ComponentType)(it - ComponentTypeStr.begin());
	}
	else {
		Debug::Warning("PrefabComponent", "Unknown type: \"" + json.key.string + "\"!");
	}
}

SceneObject _PrefabComp::Instantiate(const SceneObject& o) const {
#define CS(tp) case ComponentType::tp: Instantiate ## tp(o); break;
	switch (type) {
		CS(Camera)
		CS(Animator)
		CS(Light)
		CS(MeshRenderer)
		CS(Rig)
	default:
		CE_NOT_IMPLEMENTED
	}

	return nullptr;
}

CE_END_NAMESPACE