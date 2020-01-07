#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
	void RegActive(const std::string& path) {
		ESceneManager::activeScenePath = path;
		std::ofstream(CE_DIR_USER + "lastopenscene.txt") << path;
	}
}

std::string ESceneManager::activeScenePath = "";

void ESceneManager::Init() {
	auto& scene = ChokoEditor::scene;
	scene->AddNewObject()
		->name("__Editor_Cameras__");
	scene->AddNewObject()
		->name("__scene__");

	ECallbackManager::Register(CallbackSig::GLOBAL_SAVE, CbFnFrom(Save));

	Ops::Reg();
}

void ESceneManager::Load(const std::string& path) {
	auto& scene = ChokoEditor::scene;
	auto json = JsonParser::Parse(IO::ReadFile(CE_DIR_ASSET + path));
	auto prb = Prefab::New(json, [](const std::string& s) -> Prefab {
		return (Prefab)EAssetList::Get(AssetType::Prefab, s, true);
	});

	scene->RemoveObject(scene->objects()[1]);
	scene->AddObject(prb->Instantiate([](AssetType t, const std::string& s) -> Asset {
		return EAssetList::Get(t, s, true);
	}));
	scene->sky((Background)EAssetManager::FromJson(json.Get("sky"), true));

	RegActive(path);
}

void ESceneManager::LoadLastOpened() {
	std::ifstream strm(CE_DIR_USER + "lastopenscene.txt");
	if (strm) {
		std::string path;
		strm >> path;
		Load(path);
	}
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

void ESceneManager::SaveAs(const std::string& path, bool reg) {
	Debug::Message("SceneManager", "Saving " + path);
	auto& scene = ChokoEditor::scene;
	auto res = Prefab::New(scene->objects()[1], true)->ToJson();

	res.group.push_back(JsonPair(JsonObject("sky"), EAssetManager::ToJson(scene->sky(), AssetType::Background)));
	std::ofstream strm(CE_DIR_ASSET + path);
	strm << JsonParser::Export(res);

	if (reg) {
		RegActive(path);
	}
}

CE_END_ED_NAMESPACE