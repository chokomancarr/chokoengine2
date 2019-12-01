#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_PrefabComp::_PrefabComp(const Component& c)
	: type(c->componentType), enabled(true) {
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

_PrefabComp::_PrefabComp(const JsonObject& json) : _PrefabObjBase(json) {
	type = (ComponentType)(std::find(ComponentTypeStr.begin(), ComponentTypeStr.end(), json.Get("type").string) - ComponentTypeStr.begin());
	enabled = json.Get("enabled").ToBool();
}

JsonObject _PrefabComp::ToJson() const {
	auto res = _PrefabObjBase::ToJson();
	res.group.push_back(JsonPair(JsonObject("type"), ComponentTypeStr[(int)type]));
	res.group.push_back(JsonPair(JsonObject("enabled"), JsonObject(enabled ? "1" : "0")));
	return res;
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