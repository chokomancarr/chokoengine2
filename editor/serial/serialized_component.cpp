#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

ESerializedComponent::ESerializedComponent(const Component& c)
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
}

JsonObject ESerializedComponent::ToJson() const {
	auto res = ESerializedObject::ToJson();
	res.group.push_back(JsonPair(JsonObject("type"), ComponentTypeStr[(int)type]));
	res.group.push_back(JsonPair(JsonObject("enabled"), JsonObject(enabled ? "1" : "0")));
	return res;
}

CE_END_ED_NAMESPACE