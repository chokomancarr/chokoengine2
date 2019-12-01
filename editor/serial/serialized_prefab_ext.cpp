#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

ESerializedPrefabExt::ESerializedPrefabExt(const SceneObject& obj, const PrefabManager::Info& info)
		: sig(info.prefab->assetSignature()), enabled(true), mods(0) {
	int i = 0;
	std::function<void(const SceneObject&)> find = [&](const SceneObject& o) {
		if (info.objects[i].lock() == o) {
			i++;
			for (auto& c : o->children()) {
				find(c);
			}
		}
		else {
			auto mod = ESerializedPrefabMod_New();
			mod->type = ESerializedPrefabMod::Type::Object;
			mod->target = CE_S_ObjectRef(o, obj);
			mod->target.path[0].second = 0;
			mod->object = ESerializedPrefab_New(o, true);
			mods.push_back(std::move(mod));
		}
	};

	find(obj);
}

ESerializedPrefabExt::ESerializedPrefabExt(const JsonObject& json) : ESerializedObject(json) {
	static const std::string TypeStr[]{
		"Modify",
		"Object"
	};

	sig = json.Get("sig").string;
	name = json.Get("name").string;
	enabled = json.Get("enabled").ToBool();
	transform.position = json.Get("position").ToVec3();
	transform.rotation = json.Get("rotation").ToQuat();
	transform.scale = json.Get("scale").ToVec3();
	for (auto& m : json.Get("mods").list) {
		auto mod = ESerializedPrefabMod_New();
		mod->type = (ESerializedPrefabMod::Type)(std::find(TypeStr, TypeStr + 2, m.Get("type").string) - TypeStr);
		mod->target = CE_S_ObjectRef(m.Get("target"));
		mod->object = ESerializedPrefab_New(m.Get("object"));
		mods.push_back(std::move(mod));
	}
}

JsonObject ESerializedPrefabExt::ToJson() const {
	auto res = ESerializedObject::ToJson();
	res.group.push_back(JsonPair(JsonObject("type"), JsonObject("link")));
	res.group.push_back(JsonPair(JsonObject("sig"), sig));
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
	}
	return res;
}

SceneObject ESerializedPrefabExt::Instantiate(const SceneObject& pr) const {
	auto res = PrefabManager::Instantiate((Prefab)EAssetList::Get(EAssetType::Prefab, sig, true));
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


JsonObject ESerializedPrefabMod::ToJson() const {
	static const std::string TypeStr[] {
		"Modify",
		"Object"
	};

	JsonObject res(JsonObject::Type::Group);
	res.group.push_back(JsonPair(JsonObject("type"), TypeStr[(int)type]));
	res.group.push_back(JsonPair(JsonObject("target"), target.ToJson()));
	res.group.push_back(JsonPair(JsonObject("object"), object->ToJson()));
	return res;
}

void ESerializedPrefabMod::Instantiate(const SceneObject& par) const {
	const auto fnd = [&](const std::vector<SceneObject>& cc, const std::pair<std::string, int>& nm) -> SceneObject {
		int n = 0;
		for (auto& c : cc) {
			if (c->name() == nm.first) {
				if (n++ == nm.second) {
					return c;
				}
			}
		}
		return nullptr;
	};

	SceneObject p = par;
	for (auto& t : target.path) {
		p = fnd(p->children(), t);
		if (!p) return;
	}

	object->Instantiate(p);
}

CE_END_ED_NAMESPACE