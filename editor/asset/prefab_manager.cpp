#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::unordered_map<ChokoEngine::objectid, Prefab> PrefabManager::_map;

SceneObject PrefabManager::Instantiate(const Prefab& prefab) {
	auto res = prefab->Instantiate();
	_map[res->id()] = prefab;
	return res;
}

static bool PrefabManager::IsPrefab(const SceneObject& o) {
	return _map.count(o->id()) > 0;
}

CE_END_ED_NAMESPACE