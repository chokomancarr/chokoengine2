#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

/* serialized object json
 * {
 *   "items":{
 *     "item_name":{
 *       "type":"type_name",
 *       "value":{ /value/ }
 *     }
 *   },
 *   { /custom data/ }
 * }
 */

ESerializedObject::ESerializedObject(const JsonObject& json) {
	auto& ii = json.Get("items");
	items.reserve(ii.group.size());
	for (auto& i : ii.group) {
		//items.emplace(i.key.string, i.value.ToInt());
	}
}

JsonObject ESerializedObject::ToJson() const {
	JsonObject its(JsonObject::Type::Group);
	if (items.empty()) return its;
	for (auto& i : items) {
		its.group.push_back(JsonPair(i.first, i.second.ToJson()));
	}
	return JsonObject({ JsonPair(JsonObject("items"), its) });
}

CE_END_ED_NAMESPACE