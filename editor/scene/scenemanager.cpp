#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
	void RegActive(const std::string& path) {
		ESceneManager::activeScenePath = path;
		std::ofstream(CE_DIR_USER + "lastopenscene.txt") << path;
	}
}

std::string ESceneManager::activeScenePath = "";

std::vector<ESceneManager::Entry> ESceneManager::scenes = {};

void ESceneManager::Init() {
	auto& scene = ChokoEditor::scene;
	scene->AddNewObject()
		->name("__Editor_Cameras__");
	scene->AddNewObject()
		->name("__scene__");

	ECallbackManager::Register(CallbackSig::GLOBAL_NEW, CbFnFrom(Unload));
	ECallbackManager::Register(CallbackSig::GLOBAL_SAVE, CbFnFrom(Save));

	Ops::Reg();

	LoadSceneList();
}

ESceneManager::_scenedatast ESceneManager::GetSceneData(const std::string& path) {
	_scenedatast res = {};
	if (!IO::FileExists(CE_DIR_ASSET + path))
		return res;
	auto json = JsonParser::Parse(IO::ReadFile(CE_DIR_ASSET + path));
	auto prb = Prefab::New(json, [](const std::string& s) -> Prefab {
		return (Prefab)EAssetList::Get(AssetType::Prefab, s, true);
	});

	res.obj = prb->Instantiate([](AssetType t, const std::string& s) -> Asset {
		return EAssetList::Get(t, s, true);
	});
	res.sky = (Background)EAssetManager::FromJson(json.Get("sky"), true);
	return res;
}

void ESceneManager::Load(const std::string& path) {
	if (!IO::FileExists(CE_DIR_ASSET + path))
		return;
	auto res = GetSceneData(path);
	auto& scene = ChokoEditor::scene;
	scene->RemoveObject(scene->objects()[1]);
	scene->AddObject(res.obj);
	scene->sky(res.sky);

	RegActive(path);
}

void ESceneManager::LoadLastOpened() {
	Load(IO::ReadFile(CE_DIR_USER + "lastopenscene.txt"));
}

void ESceneManager::Unload() {
	auto& scene = ChokoEditor::scene;
	scene->RemoveObject(sceneBaseObj());
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
	auto res = Prefab::New(sceneBaseObj(), true)->ToJson();

	res.group.push_back(JsonPair(JsonObject("sky"), EAssetManager::ToJson(scene->sky(), AssetType::Background)));
	
	std::ofstream(CE_DIR_ASSET + path) << JsonParser::Export(res);

	if (reg) {
		RegActive(path);
	}
}

const SceneObject& ESceneManager::sceneBaseObj() {
	return ChokoEditor::scene->objects()[1];
}

void ESceneManager::LoadSceneList() {
	auto json = JsonParser::Parse(IO::ReadFile(CE_DIR_CONFIG + "scenes.json"));
	for (auto& g : json.group[0].value.group) {
		Entry e;
		e.name = g.key.string;
		e.sig = g.value.Get("sig").string;
		e.enabled = g.value.Get("enabled").ToBool();
		scenes.push_back(e);
	}
}
void ESceneManager::WriteSceneList() {
	JsonObject scns(JsonObject::Type::Group);
	for (auto& s : scenes) {
		JsonObject scn({
			JsonPair(JsonObject("sig"), s.sig),
			JsonPair(JsonObject("enabled"), JsonObject(s.enabled ? "1" : "0"))
		});
		scns.group.push_back(JsonPair(s.name, scn));
	}
	JsonObject obj({
		JsonPair(JsonObject("scenes"), scns)
	});
}

std::string ESceneManager::GetRelSceneIndex() {
	std::string res = "";
	for (auto& e : scenes) {
		if (e.enabled) {
			res += e.name + "\n";
		}
	}
	return res;
}
std::vector<std::vector<byte>> ESceneManager::GetRelScenes() {
	std::vector<std::vector<byte>> res = {};
	for (auto& e : scenes) {
		if (e.enabled) {
			res.push_back(IO::ReadFileBinary(CE_DIR_ASSET + e.sig));
		}
	}
	return res;
}

CE_END_ED_NAMESPACE