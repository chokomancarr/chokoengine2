#include "prefab/prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_Prefab::_Prefab(const SceneObject& o, bool link) : _data(new _PrefabObj(o, link)) {}

SceneObject _Prefab::Instantiate() const {
	return _data->Instantiate(nullptr);
}

CE_END_NAMESPACE