#include "chokoplayer.hpp"

CE_BEGIN_PL_NAMESPACE

namespace {
    Scene _activeScene = nullptr;
}

std::vector<std::string> SceneManager::scenePaths = {};

const Scene& SceneManager::activeScene() {
    return _activeScene;
}

void SceneManager::Init() {

}

void SceneManager::Load(int i) {
	auto json = JsonParser::Parse(IO::ReadFile(scenePaths[i]));
	auto prb = Prefab::New(json, [](const std::string& s) -> Prefab {
		return (Prefab)_AssetLoaderBase::instance->Load(AssetType::Prefab, s);
	});

    auto res = prb->Instantiate([](AssetType t, const std::string& s) -> Asset {
        return _AssetLoaderBase::instance->Load(t, s);
	});
    for (auto& o : res->children()) {
        o->parent(nullptr);
        _activeScene->AddObject(o);
    }

	//scene->sky((Background)EAssetManager::FromJson(json.Get("sky"), true));
}

CE_END_PL_NAMESPACE