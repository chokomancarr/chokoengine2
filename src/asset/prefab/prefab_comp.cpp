#include "prefab_inc.hpp"
#include "ce2/scene/comp/detail/compdefsall.inl"

CE_BEGIN_NAMESPACE

_PrefabComp::_PrefabComp(const Component& c)
		: _PrefabObjBase(Type::Comp), type(c->componentType) {
	name = ComponentTypeStr.at(type);
#define CS(tp) case ComponentType::tp: Set ## tp((tp)c); break;
	switch (type) {
		COMP_DEFS_ALL(CS)
	default:
		CE_NOT_IMPLEMENTED
	}
#undef CS
}

_PrefabComp::_PrefabComp(const JsonPair& json) : _PrefabObjBase(Type::Comp, json.value) {
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

SceneObject _PrefabComp::Instantiate(const SceneObject& o) {
#define CS(tp) case ComponentType::tp: Instantiate ## tp(o); break;
	switch (type) {
		COMP_DEFS_ALL(CS)
	default:
		CE_NOT_IMPLEMENTED
	}

	return nullptr;
}

void _PrefabComp::Apply(const Component& c) const {
#define CS(tp) case ComponentType::tp: Apply ## tp((tp)c); break;
	switch (c->componentType) {
		COMP_DEFS_ALL(CS)
	default:
		CE_NOT_IMPLEMENTED
	}
}

#undef SWITCH_COMPS

CE_END_NAMESPACE