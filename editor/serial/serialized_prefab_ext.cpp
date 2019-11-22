#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

ESerializedPrefabExt::ESerializedPrefabExt(const SceneObject& obj, const PrefabManager::Info& info)
		: sig(info.prefab->assetSignature()), enabled(true), mods(0) {
	int i = 1;
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
			mod->object = ESerializedPrefab_New(obj);
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
	
	return res;
}

CE_END_ED_NAMESPACE