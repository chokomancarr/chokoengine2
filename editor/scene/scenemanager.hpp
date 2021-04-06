#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ESceneManager {
public:
	static std::string activeScenePath;

	struct Entry {
		std::string name;
		std::string sig;
		bool enabled;
	};
	static std::vector<Entry> scenes;

	static void Init();

	static struct _scenedatast {
		SceneObject obj;
		Background sky;
	} GetSceneData(const std::string& path);

	static void Load(const std::string& path);
	static void LoadLastOpened();

	static void Unload();

	static bool Save();
	
	static void SaveAs(const std::string& path, bool regActive = true);

	static const SceneObject& sceneBaseObj();

	static void LoadSceneList();
	static void WriteSceneList();

	static std::string GetRelSceneIndex();
	static std::vector<std::vector<byte>> GetRelScenes();

	class Ops;
};

CE_END_ED_NAMESPACE

#include "ops/scene_ops.hpp"
