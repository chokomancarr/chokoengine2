#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

/* I dunno if this works
 */
class PrefabManager {
	struct PrefabInfo {
		Prefab prefab;
		std::vector<pSceneObject> objects;
	};
	static std::unordered_map<ChokoEngine::objectid, PrefabInfo> _map;
public:
	static SceneObject Instantiate(const Prefab&);

	static bool IsPrefab(const SceneObject&);

	static void Apply(const SceneObject&);
};

CE_END_ED_NAMESPACE