#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_PrefabMod::_PrefabMod(const PrefabObj& obj, const SceneObject& tar, const SceneObject& root) {
	target = Prefab_ObjRef(tar, root);

	using itp = std::pair<std::string, PrefabItem>;
	auto cgsp = std::find_if(obj->items.begin(), obj->items.end(), [](const itp& i) {
		return i.first == "components";
	});
	//auto cgs = (cgsp == obj->items.end()) ? PrefabObjGroup() : cgsp->second.value.objgroup;
	for (auto& c : tar->components()) {
		
	}
}

JsonObject _PrefabMod::ToJson() const {
	return JsonObject();
}

CE_END_NAMESPACE