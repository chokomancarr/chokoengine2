#include "prefab/prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_Prefab::_Prefab(const SceneObject& o, bool link) : _data(new _PrefabObj(o, link)) {}

_Prefab::_Prefab(const JsonObject& json, _Sig2Prb fn) {
	PrefabState::sig2PrbFn = fn;
	if (json.group[0].key.string != "object") {
		Debug::Warning("Prefab", "Cannot create from json: no object entry!");
		_deleted = true;
		return;
	}
	_data = PrefabObj_New(json.group[0].value);
}

JsonObject _Prefab::ToJson() const {
	return JsonObject({ _data->ToJson() });
}

SceneObject _Prefab::Instantiate(_Sig2Ass fn) const {
	PrefabState::sig2AssFn = fn;
	return _data->Instantiate(nullptr);
}

CE_END_NAMESPACE