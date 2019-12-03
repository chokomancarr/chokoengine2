#include "prefab/prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_Prefab::_Prefab(const SceneObject& o, bool link) : _data(new _PrefabObj(o, link)) {}

_Prefab::_Prefab(const JsonObject& json, _Sig2Prb fn) {
	PrefabState::sig2PrbFn = fn;
	_data = PrefabObj_New(json);
}

JsonObject _Prefab::ToJson() const {
	return _data->ToJson();
}

SceneObject _Prefab::Instantiate(_Sig2Ass fn) const {
	PrefabState::sig2AssFn = fn;
	return _data->Instantiate(nullptr);
}

CE_END_NAMESPACE