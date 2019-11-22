#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

PrefabManager::Info PrefabManager::Info::null = {};

bool PrefabManager::Info::operator!() const {
	return this == &null;
}


std::unordered_map<ChokoEngine::objectid, PrefabManager::Info> PrefabManager::_map;

SceneObject PrefabManager::Instantiate(const Prefab& prefab) {
	typedef std::vector<pSceneObject>& _sos;

	std::function<void(_sos, const SceneObject&)> rego = [&rego](_sos ov, const SceneObject& o) {
		ov.push_back(pSceneObject(o));
		for (auto& c : o->children()) {
			rego(ov, c);
		}
	};
	
	auto res = prefab->Instantiate();
	auto& info = _map[res->id()];
	info.prefab = prefab;
	rego(info.objects, res);
	return res;
}

PrefabManager::Info& PrefabManager::GetInfo(const SceneObject& o) {
	auto it = _map.find(o->id());
	if (it == _map.end()) return Info::null;
	else return it->second;
}

bool PrefabManager::IsPrefab(const SceneObject& o) {
	return _map.count(o->id()) > 0;
}

void PrefabManager::Apply(const SceneObject& o) {

}

CE_END_ED_NAMESPACE