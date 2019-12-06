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
	auto prb = Prefab::New(json, [](const std::string& s) -> Prefab {
		return (Prefab)EAssetList::Get(AssetType::Prefab, s, true);
	});

	//scene->RemoveObject(scene->objects()[1]);
	//scene->AddObject(prb->Instantiate(nullptr));
}

void ESceneManager::Unload() {
	auto& scene = ChokoEditor::scene;
	scene->RemoveObject(scene->objects()[1]);
	scene->AddNewObject()
		->name("__scene__");
}

void ESceneManager::Save(const std::string& path) {
	auto& scene = ChokoEditor::scene;
	auto res = Prefab::New(scene->objects()[1], true)->ToJson();
	std::ofstream strm(path);
	strm << JsonParser::Export(res);
}

CE_END_ED_NAMESPACE