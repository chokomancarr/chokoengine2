#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

class _PrefabMod {
public:
	//commands to modify object saved as obj to become tar
	_PrefabMod(const _PrefabObj* obj, const SceneObject& tar, const SceneObject& root);
	_PrefabMod(const _Prefab::_ObjTreeBase& obj, const SceneObject& tar, const SceneObject& root);

	_PrefabMod(const JsonObject&);

	Prefab_ObjRef target;

	std::vector<std::pair<int, PrefabComp>> add_comps;
	std::vector<PrefabComp> mod_comps;

	bool empty() const;

	JsonObject ToJson() const;

	void Instantiate(const SceneObject&) const;
};

CE_END_NAMESPACE