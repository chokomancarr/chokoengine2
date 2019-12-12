#include "chokoplayer.hpp"

CE_BEGIN_PL_NAMESPACE

namespace {
    std::vector<std::string> _paths = {};

    Scene _activeScene = nullptr;
}

const Scene& SceneManager::activeScene() {
    return _activeScene;
}

void SceneManager::Init() {

}

void SceneManager::Load(int i) {
	auto json = JsonParser::Parse(IO::ReadFile(ChokoPlayer::projectPath + _paths[i]));
	auto prb = Prefab::New(json, [](const std::string& s) -> Prefab {
		return nullptr; //(Prefab)EAssetList::Get(AssetType::Prefab, s, true);
	});

    auto res = prb->Instantiate([](AssetType t, const std::string& s) -> Asset {
        return nullptr; //EAssetList::Get(t, s, true);
	});
    for (auto& o : res->children()) {
        o->parent(nullptr);
        _activeScene->AddObject(o);
    }

	//scene->sky((Background)EAssetManager::FromJson(json.Get("sky"), true));
}

CE_END_PL_NAMESPACE