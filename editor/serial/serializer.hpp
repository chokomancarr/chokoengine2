#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ESerializer {
	static std::unordered_map<ChokoEngine::objectid, int> objectToSceneId;

public:
	static void UpdateSceneIds();

	static int SceneIdOf(const SceneObject&);
	static int SceneIdOf(ChokoEngine::objectid);
};

CE_END_ED_NAMESPACE