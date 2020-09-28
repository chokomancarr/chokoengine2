#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

class _PrefabLink : public _PrefabObjBase {
	Prefab tar;

public:
	_PrefabLink(const SceneObject&, const SceneObject&, bool mod);

	_PrefabLink(const JsonObject&);

	Prefab& GetTarget();

	void LoadLinks() override;

	SceneObject Instantiate(const SceneObject&) override;
};

CE_END_NAMESPACE