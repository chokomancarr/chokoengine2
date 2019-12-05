#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

class _PrefabObj : public _PrefabObjBase {
public:
	_PrefabObj(const SceneObject&, bool uselinks);

	_PrefabObj(const JsonObject&);
	/*
	std::string name;
	bool enabled;
	struct {
		Vec3 position;
		Quat rotation;
		Vec3 scale;
	} transform;

	std::vector<PrefabComp> components;

	std::vector<PrefabObjBase> children;
	*/

	SceneObject Instantiate(const SceneObject&) const override;
};

CE_END_NAMESPACE