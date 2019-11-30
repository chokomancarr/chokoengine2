#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ESerializedPrefabExt : public ESerializedObject {
public:
	ESerializedPrefabExt(const SceneObject&, const PrefabManager::Info&);

	ESerializedPrefabExt(const JsonObject&);

	std::string sig;

	std::string name;
	bool enabled;
	struct {
		Vec3 position;
		Quat rotation;
		Vec3 scale;
	} transform;

	std::vector<pESerializedPrefabMod> mods;

	JsonObject ToJson() const override;

	SceneObject Instantiate(const SceneObject&) const override;
};

class ESerializedPrefabMod {
public:
	enum class Type {
		Modify,
		Object
	} type;

	CE_S_ObjectRef target;

	pESerializedPrefab object;

	JsonObject ToJson() const;

	void Instantiate(const SceneObject&) const;
};

CE_END_ED_NAMESPACE