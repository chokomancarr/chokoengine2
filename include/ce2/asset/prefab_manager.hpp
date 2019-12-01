#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class PrefabManager { CE_CLASS_COMMON
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

CE_END_NAMESPACE