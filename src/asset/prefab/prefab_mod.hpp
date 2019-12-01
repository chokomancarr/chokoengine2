#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

class _PrefabMod {
public:
	enum class Type {
		Modify,
		Object
	} type;

	Prefab_ObjRef target;

	PrefabObj object;

	JsonObject ToJson() const;

	void Instantiate(const SceneObject&) const;
};

CE_END_NAMESPACE