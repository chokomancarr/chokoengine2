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
JsonObject ESerializedObject::ToJson() const {
	JsonObject its(JsonObject::Type::Group);
	if (items.empty()) return its;
	for (auto& i : items) {
		its.group.push_back(JsonPair(i.first, i.second.ToJson()));
	}
	return JsonObject({ JsonPair(JsonObject("items"), its) });
}

CE_END_ED_NAMESPACE