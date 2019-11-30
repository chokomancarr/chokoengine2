#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void ESceneManager::Init() {
	auto& scene = ChokoEditor::scene;
	scene->AddNewObject()
		->name("__Editor_Cameras__");
	scene->AddNewObject()
		->name("__scene__");
}

void ESceneManager::Load(const std::string& path) {
	auto& scene = ChokoEditor::scene;
	auto json = JsonParser::Parse(IO::ReadFile(ChokoEditor::assetPath + path));
	auto prb = ESerializedPrefab_New(json);

	scene->RemoveObject(scene->objects()[1]);
	scene->AddObject(prb->Instantiate(nullptr));
}

void ESceneManager::Unload() {
	auto& scene = ChokoEditor::scene;
	scene->RemoveObject(scene->objects()[1]);
	scene->AddNewObject()
		->name("__scene__");
}

CE_END_ED_NAMESPACE