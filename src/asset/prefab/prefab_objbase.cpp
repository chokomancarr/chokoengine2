#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_Prefab::_ObjBase::_ObjBase(const JsonObject& json) {
	for (auto& i : json.group) {
		auto lc = i.key.string.find_last_of('.');
		items.push_back(std::make_pair(i.key.string.substr(0, lc),
			PrefabItem(i.key.string.substr(lc+1), i.value)));
	}
}

JsonPair _Prefab::_ObjBase::ToJson() const {
	JsonObject res(JsonObject::Type::Group);
	for (auto& i : items) {
		res.group.push_back(i.second.ToJson(i.first));
	}
	return JsonPair(JsonObject(name), res);
}

CE_END_NAMESPACE