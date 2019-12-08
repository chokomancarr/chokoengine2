#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

class _PrefabObj : public _PrefabObjBase {
public:
	_PrefabObj(const SceneObject&, const SceneObject&, bool uselinks, bool firstlink, bool first);

	_PrefabObj(const JsonObject&);

	SceneObject Instantiate(const SceneObject&) const override;
};

CE_END_NAMESPACE