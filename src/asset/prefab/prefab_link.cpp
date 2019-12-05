#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_PrefabLink::_PrefabLink(const SceneObject& obj)
	: tar(obj->prefab().lock()), enabled(true), mods(0) {
	int i = 0;
	std::function<void(const SceneObject&)> find = [&](const SceneObject& o) {
		if (o->prefab() == tar) {
			i++;
			for (auto& c : o->children()) {
				find(c);
			}
		}
		else {
			if (!o->prefab()) {
				auto mod = PrefabMod_New();
				mod->type = _PrefabMod::Type::Object;
				mod->target = Prefab_ObjRef(o, obj);
				mod->target.path[0].second = 0;
				mod->object = PrefabObj_New(o, true);
				mods.push_back(std::move(mod));
			}
			else {
				CE_NOT_IMPLEMENTED
			}
		}
	};

	find(obj);
}

_PrefabLink::_PrefabLink(const JsonObject& json) : _PrefabObjBase(json) {
	static const std::string TypeStr[]{
		"Modify",
		"Object"
	};

	tar = PrefabState::sig2PrbFn(json.Get("sig").string);
	name = json.Get("name").string;
	enabled = json.Get("enabled").ToBool();
	transform.position = json.Get("position").ToVec3();
	transform.rotation = json.Get("rotation").ToQuat();
	transform.scale = json.Get("scale").ToVec3();
	for (auto& m : json.Get("mods").list) {
		auto mod = PrefabMod_New();
		mod->type = (_PrefabMod::Type)(std::find(TypeStr, TypeStr + 2, m.Get("type").string) - TypeStr);
		mod->target = Prefab_ObjRef(m.Get("target"));
		mod->object = PrefabObj_New(m.Get("object"));
		mods.push_back(std::move(mod));
	}
}

JsonPair _PrefabLink::ToJson() const {
	auto res = _PrefabObjBase::ToJson();/*
	res.group.push_back(JsonPair(JsonObject("type"), JsonObject("link")));
	res.group.push_back(JsonPair(JsonObject("sig"), tar->assetSignature()));
	res.group.push_back(JsonPair(JsonObject("name"), name));
	res.group.push_back(JsonPair(JsonObject("enabled"), JsonObject(enabled ? "1" : "0")));
	res.group.push_back(JsonPair(JsonObject("position"), JsonObject::FromVec3(transform.position)));
	res.group.push_back(JsonPair(JsonObject("rotation"), JsonObject::FromQuat(transform.rotation)));
	res.group.push_back(JsonPair(JsonObject("scale"), JsonObject::FromVec3(transform.scale)));
	if (!mods.empty()) {
		JsonObject chds(JsonObject::Type::List);
		for (auto& c : mods) {
			chds.list.push_back(c->ToJson());
		}
		res.group.push_back(JsonPair(JsonObject("mods"), chds));
	}*/
	return res;
}

SceneObject _PrefabLink::Instantiate(const SceneObject& pr) const {
	auto res = tar->Instantiate(PrefabState::sig2AssFn);
	res->name(name);
	res->transform()->localPosition(transform.position);
	res->transform()->localRotation(transform.rotation);
	res->transform()->localScale(transform.scale);

	res->parent(pr);

	for (auto& m : mods) {
		m->Instantiate(res);
	}

	return res;
}

CE_END_NAMESPACE