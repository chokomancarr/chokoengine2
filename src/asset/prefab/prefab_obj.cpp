#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_PrefabObj::_PrefabObj(const SceneObject& o, bool lnk)
		: name(o->name()), enabled(true), transform({
			o->transform()->localPosition(),
			o->transform()->localRotation(),
			o->transform()->localScale()
			}), components(0), children(0) {
	auto& cc = o->components();
	components.reserve(cc.size());
	for (auto& c : cc) {
		components.push_back(PrefabComp_New(c));
	}
	auto& cch = o->children();
	children.reserve(cch.size());
	for (auto& c : cch) {
		if (lnk) {
			if (!!c->prefab()) {
				children.push_back(PrefabLink_New(c));
				continue;
			}
		}
		children.push_back(PrefabObj_New(c, lnk));
	}
}

_PrefabObj::_PrefabObj(const JsonObject& json) : _PrefabObjBase(json) {
	name = json.Get("name").string;
	enabled = json.Get("enabled").ToBool();
	transform.position = json.Get("position").ToVec3();
	transform.rotation = json.Get("rotation").ToQuat();
	transform.scale = json.Get("scale").ToVec3();
	for (auto& c : json.Get("components").list) {
		components.push_back(PrefabComp_New(c));
	}
	for (auto& c : json.Get("children").list) {
		if (c.Get("type").string == "link") {
			children.push_back(PrefabLink_New(c));
		}
		else
			children.push_back(PrefabObj_New(c));
	}
}

JsonObject _PrefabObj::ToJson() const {
	auto res = JsonObject(JsonObject::Type::Group);
	res.group.push_back(JsonPair(JsonObject("name"), name));
	res.group.push_back(JsonPair(JsonObject("enabled"), JsonObject(enabled ? "1" : "0")));
	res.group.push_back(JsonPair(JsonObject("position"), JsonObject::FromVec3(transform.position)));
	res.group.push_back(JsonPair(JsonObject("rotation"), JsonObject::FromQuat(transform.rotation)));
	res.group.push_back(JsonPair(JsonObject("scale"), JsonObject::FromVec3(transform.scale)));
	if (!components.empty()) {
		JsonObject cmps(JsonObject::Type::Group);
		for (auto& c : components) {
			cmps.group.push_back(JsonPair(JsonObject("components"), c->ToJson()));
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
	return JsonObject({JsonPair(JsonObject("object"), res)});
}

SceneObject _PrefabObj::Instantiate(const SceneObject& pr) const {
	SceneObject res = SceneObject::New(name, transform.position, transform.rotation, transform.scale);
	for (auto& c : components) {
		c->Instantiate(res);
	}
	for (auto& c : children) {
		c->Instantiate(res);
	}

	res->parent(pr);

	return res;
}

CE_END_NAMESPACE