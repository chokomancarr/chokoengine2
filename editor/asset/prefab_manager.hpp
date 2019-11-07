#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

/* I dunno if this works
 */
class PrefabManager {
	static std::unordered_map<ChokoEngine::objectid, Prefab> _map;
public:
	static SceneObject Instantiate(const Prefab&);

	static bool IsPrefab(const SceneObject&);
};

CE_END_ED_NAMESPACE