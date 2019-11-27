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
			mod->target = CE_S_ObjectRef(o->parent().lock(), obj->parent().lock());
			mod->target.path[0].second = 0;
			mod->object = ESerializedPrefab_New(o);
			mods.push_back(std::move(mod));
		}
	};

	find(obj);
}

JsonObject ESerializedPrefabExt::ToJson() const {
	auto res = ESerializedObject::ToJson();
	res.group.push_back(JsonPair(JsonObject("type"), JsonObject("link")));
	res.group.push_back(JsonPair(JsonObject("sig"), sig));
	res.group.push_back(JsonPair(JsonObject("enabled"), JsonObject(enabled ? "1" : "0")));
	if (!mods.empty()) {
		JsonObject chds(JsonObject::Type::List);
		for (auto& c : mods) {
			chds.list.push_back(c->ToJson());
		}
		res.group.push_back(JsonPair(JsonObject("mods"), chds));
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

CE_END_ED_NAMESPACE