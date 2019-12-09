#include "prefab/prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_Prefab::_Prefab(const SceneObject& o, bool link) : _Asset(AssetType::Prefab) {
	PrefabState::prefabStack.push_back(o->prefab());
	_data = PrefabObj(new _PrefabObj(o, o->parent().lock(), link, true, true));
	PrefabState::prefabStack.pop_back();
}

_Prefab::_Prefab(const JsonObject& json, _Sig2Prb fn) : _Asset(AssetType::Prefab) {
	PrefabState::sig2PrbFn = fn;
	if (json.group[0].key.string != "object") {
		Debug::Warning("Prefab", "Cannot create from json: no object entry!");
		_deleted = true;
		return;
	}
	_data = PrefabObj_New(json.group[0].value);
}

_Prefab::~_Prefab() {}

JsonObject _Prefab::ToJson() const {
	return JsonObject({ _data->ToJson() });
}

SceneObject _Prefab::Instantiate(_Sig2Ass fn) const {
	PrefabState::sig2AssFn = fn;
	PrefabState::prefabStack.push_back(get_shared<_Prefab>());
	PrefabState::activeBaseObjs.push(nullptr);
	const auto& res = _data->Instantiate(nullptr);
	PrefabState::activeBaseObjs.pop();
	PrefabState::prefabStack.pop_back();
	return res;
}

CE_END_NAMESPACE