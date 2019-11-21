#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::unordered_map<ChokoEngine::objectid, PrefabManager::PrefabInfo> PrefabManager::_map;

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

bool PrefabManager::IsPrefab(const SceneObject& o) {
	return _map.count(o->id()) > 0;
}

void PrefabManager::Apply(const SceneObject& o) {

}

CE_END_ED_NAMESPACE