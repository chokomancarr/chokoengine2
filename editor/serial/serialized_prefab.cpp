#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

ESerializedPrefab::ESerializedPrefab(const SceneObject& o)
		: name(o->name()), enabled(true), transform({
			o->transform()->localPosition(),
			o->transform()->localRotation(),
			o->transform()->localScale()
			}), components(0), children(0) {
	auto& cc = o->components();
	components.reserve(cc.size());
	for (auto& c : cc) {
		components.push_back(ESerializedComponent_New(c));
	}
	auto& cch = o->children();
	children.reserve(cch.size());
	for (auto& c : cch) {
		children.push_back(ESerializedPrefab_New(c));
	}
}

JsonObject ESerializedPrefab::ToJson() const {
	auto res = ESerializedObject::ToJson();
	res.group.push_back(JsonPair(JsonObject("name"), name));
	res.group.push_back(JsonPair(JsonObject("enabled"), JsonObject(enabled ? "1" : "0")));
	res.group.push_back(JsonPair(JsonObject("position"), JsonObject::FromVec3(transform.position)));
	res.group.push_back(JsonPair(JsonObject("rotation"), JsonObject::FromQuat(transform.rotation)));
	res.group.push_back(JsonPair(JsonObject("scale"), JsonObject::FromVec3(transform.scale)));
	if (!components.empty()) {
		JsonObject cmps(JsonObject::Type::List);
		for (auto& c : components) {
			cmps.list.push_back(c->ToJson());
		}
		res.group.push_back(JsonPair(JsonObject("components"), cmps));
	}
	if (!children.empty()) {
		JsonObject chds(JsonObject::Type::List);
		for (auto& c : children) {
			chds.list.push_back(c->ToJson());
		}
		res.group.push_back(JsonPair(JsonObject("children"), chds));
	}
	return res;
}

CE_END_ED_NAMESPACE