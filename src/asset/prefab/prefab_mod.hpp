#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

class _PrefabMod {
public:
	//commands to modify object saved as obj to become tar
	_PrefabMod(const PrefabObj& obj, const SceneObject& tar, const SceneObject& root);

	Prefab_ObjRef target;

	std::vector<std::pair<int, PrefabComp>> add_comps;

	JsonObject ToJson() const;
};

CE_END_NAMESPACE