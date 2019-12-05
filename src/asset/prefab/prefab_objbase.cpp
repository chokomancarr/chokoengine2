#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_Prefab::_ObjBase::_ObjBase(const JsonObject& json) {
	for (auto& i : json.group) {
		items.push_back(std::make_pair(i.key.string, PrefabItem(i.value)));
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