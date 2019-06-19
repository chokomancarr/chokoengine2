#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ESceneInfo {


public:
	static SceneObject selectedObject;
	static std::vector<SceneObject> subSelectedObjects;
};

CE_END_ED_NAMESPACE