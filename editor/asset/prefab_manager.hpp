#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

/* I dunno if this works
 */
class PrefabManager {
public:
	class Info {
	public:
		static Info null;

		Prefab prefab;
		std::vector<pSceneObject> objects;

		bool operator !() const;
	};

private:
	static std::unordered_map<ChokoEngine::objectid, Info> _map;

public:
	static SceneObject Instantiate(const Prefab&);

	static Info& GetInfo(const SceneObject&);
	static bool IsPrefab(const SceneObject&);

	static void Apply(const SceneObject&);
};

CE_END_ED_NAMESPACE