#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ESerializedPrefabExt : public ESerializedObject {
public:
	ESerializedPrefabExt(const SceneObject&, const PrefabManager::Info&);

	std::string sig;

	bool enabled;

	std::vector<pESerializedPrefabMod> mods;

	JsonObject ToJson() const override;
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
};

CE_END_ED_NAMESPACE