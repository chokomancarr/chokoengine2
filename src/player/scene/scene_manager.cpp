#include "chokoplayer.hpp"

CE_BEGIN_PL_NAMESPACE

namespace {
    Scene _activeScene = nullptr;
}

std::vector<SceneManager::Entry> SceneManager::scenes = {};

const Scene& SceneManager::activeScene() {
    return _activeScene;
}

void SceneManager::Init() {

}

Scene SceneManager::Load(int i) {
	_activeScene = Scene::New();
	auto json = JsonParser::Parse(IO::ReadFile(scenes[i].path));
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

	auto sjson = json.Get("sky");
	_activeScene->sky((Background)_AssetLoaderBase::instance->Load(
		AssetType::Background, sjson.Get("Background").string));

	return _activeScene;
}

Scene SceneManager::Load(const std::string& s) {
	auto it = std::find_if(scenes.begin(), scenes.end(), [&s](const Entry& e) {
		return e.name == s;
	});
	if (it == scenes.end()) return nullptr;
	return Load(it - scenes.begin());
}

CE_END_PL_NAMESPACE