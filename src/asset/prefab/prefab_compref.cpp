#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

Prefab_CompRef::Prefab_CompRef(const Component& tar, const SceneObject& base)
	: objref(tar->object(), base), type(tar->componentType) {}

Prefab_CompRef::Prefab_CompRef(const JsonObject& json)
		: objref(json.Get("object")) {
	const auto& cnm = json.Get("component").string;
	auto it = std::find_if(ComponentTypeStr.begin(), ComponentTypeStr.end(),
		[&cnm](decltype(ComponentTypeStr)::const_reference i) {
		return i.second == cnm;
	});
	if (it != ComponentTypeStr.end()) {
		type = it->first;
	}
	else {
		Debug::Warning("PrefabComponentRef", "Unknown type: \"" + cnm + "\"!");
	}
}

Component Prefab_CompRef::Seek(const std::vector<SceneObject>& objs) const {
	const auto& obj = objref.Seek(objs);
	const auto& comps = obj->components();
	const auto it = std::find_if(comps.begin(), comps.end(), [&](const Component& c) {
		return c->componentType == type;
	});
	return (it == comps.end()) ? nullptr : *it;
}

JsonObject Prefab_CompRef::ToJson() const {
	return JsonObject({
		JsonPair(JsonObject("object"), objref.ToJson()),
		JsonPair(JsonObject("component"), ComponentTypeStr.at(type))
	});
}

CE_END_NAMESPACE