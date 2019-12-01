#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

class _PrefabLink : public _PrefabObjBase {
public:
	_PrefabLink(const SceneObject&, const PrefabManager::Info&);

	_PrefabLink(const JsonObject&);

	std::string sig;

	std::string name;
	bool enabled;
	struct {
		Vec3 position;
		Quat rotation;
		Vec3 scale;
	} transform;

	std::vector<PrefabMod> mods;

	JsonObject ToJson() const override;

	SceneObject Instantiate(const SceneObject&) const override;
};

CE_END_NAMESPACE