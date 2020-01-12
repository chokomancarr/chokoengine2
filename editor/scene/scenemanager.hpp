#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ESceneManager {

public:
	static std::string activeScenePath;

	static void Init();

	static void Load(const std::string& path);
	static void LoadLastOpened();

	static void Unload();

	static bool Save();
	
	static void SaveAs(const std::string& path, bool regActive = true);

	static const SceneObject& sceneBaseObj();

	class Ops;
};

CE_END_ED_NAMESPACE

#include "ops/scene_ops.hpp"
