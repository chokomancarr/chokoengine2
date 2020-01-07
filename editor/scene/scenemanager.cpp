#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::string ESceneManager::activeScenePath = "";

void ESceneManager::Init() {
	auto& scene = ChokoEditor::scene;
	scene->AddNewObject()
		->name("__Editor_Cameras__");
	scene->AddNewObject()
		->name("__scene__");

	ECallbackManager::Register(CallbackSig::GLOBAL_SAVE, CbFnFrom(Save));
}

void ESceneManager::Load(const std::string& path) {
	auto& scene = ChokoEditor::scene;
	auto json = JsonParser::Parse(IO::ReadFile(ChokoEditor::assetPath + path));
	auto prb = Prefab::New(json, [](const std::string& s) -> Prefab {
		return (Prefab)EAssetList::Get(AssetType::Prefab, s, true);
	});

	scene->RemoveObject(scene->objects()[1]);
	scene->AddObject(prb->Instantiate([](AssetType t, const std::string& s) -> Asset {
		return EAssetList::Get(t, s, true);
	}));
	scene->sky((Background)EAssetManager::FromJson(json.Get("sky"), true));

	activeScenePath = path;
}

void ESceneManager::Unload() {
	auto& scene = ChokoEditor::scene;
	scene->RemoveObject(scene->objects()[1]);
	scene->AddNewObject()
		->name("__scene__");

	activeScenePath = "";
}

bool ESceneManager::Save() {
	if (activeScenePath.empty()) return false;

	SaveAs(activeScenePath);
	return true;
}

void ESceneManager::SaveAs(const std::string& path) {
	Debug::Message("SceneManager", "Saving " + path);
	auto& scene = ChokoEditor::scene;
	auto res = Prefab::New(scene->objects()[1], true)->ToJson();

	res.group.push_back(JsonPair(JsonObject("sky"), EAssetManager::ToJson(scene->sky(), AssetType::Background)));
	std::ofstream strm(ChokoEditor::assetPath + path);
	strm << JsonParser::Export(res);
}

CE_END_ED_NAMESPACE