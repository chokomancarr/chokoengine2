#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ESceneInfo {


public:
	static SceneObject selectedObject;
	static std::vector<SceneObject> subSelectedObjects;

	static Asset selectedAsset;
};

CE_END_ED_NAMESPACE