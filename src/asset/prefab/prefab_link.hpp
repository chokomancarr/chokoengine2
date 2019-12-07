#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

class _PrefabLink : public _PrefabObjBase {
public:
	_PrefabLink(const SceneObject&, bool mod);

	_PrefabLink(const JsonObject&);

	Prefab tar;

	SceneObject Instantiate(const SceneObject&) const override;
};

CE_END_NAMESPACE