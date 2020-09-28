#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_Prefab::_ObjBase::_ObjBase(Type t) : _type(t) {}

_Prefab::_ObjBase::_ObjBase(Type t, const JsonObject& json) : _type(t) {
	for (auto& i : json.group) {
		auto lc = i.key.string.find_last_of('.');
		const auto nm = i.key.string.substr(0, lc);
		if (nm == "mods") {
			for (auto& g : i.value.list) {
				mods.push_back(PrefabMod_New(g));
			}
		}
		else {
			items.push_back(std::make_pair(nm,
				PrefabItem(i.key.string.substr(lc + 1), i.value)));
		}
	}
}

JsonPair _Prefab::_ObjBase::ToJson() const {
	JsonObject res(JsonObject::Type::Group);
	for (auto& i : items) {
		res.group.push_back(i.second.ToJson(i.first));
	}

	if (!mods.empty()) {
		JsonObject mds(JsonObject::Type::List);
		for (auto& m : mods) {
			mds.list.push_back(m->ToJson());
		}
		res.group.push_back(JsonPair(JsonObject("mods.ItemGroup*"), mds));
	}
	return JsonPair(JsonObject(name), res);
}

CE_END_NAMESPACE