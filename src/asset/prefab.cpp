#include "prefab/prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_Prefab::_Prefab(const SceneObject& o, bool link) : _Asset(AssetType::Prefab) {
	PrefabState::activeBaseObjs.push(o);
	_data = PrefabObj_New(o, o->parent().lock(), link, true, true);
	PrefabState::activeBaseObjs.pop();
}

_Prefab::_Prefab(const JsonObject& json, _Sig2Prb fn) : _Asset(AssetType::Prefab) {
	PrefabState::sig2PrbFn = fn;
	if (json.group[0].key.string != "object") {
		Debug::Warning("Prefab", "Cannot create from json: no object entry!");
		_deleted = true;
		return;
	}
	_data = PrefabObj_New(json.group[0].value);
}

_Prefab::~_Prefab() {}

JsonObject _Prefab::ToJson() const {
	return JsonObject({ _data->ToJson() });
}

SceneObject _Prefab::Instantiate(_Sig2Ass fn) const {
	PrefabState::sig2AssFn = fn;
	//PrefabState::prefabStack.push_back(get_shared<_Prefab>());
	PrefabState::activeBaseObjs.push(nullptr);
	PrefabState::refresolvers.push({});
	const auto& res = _data->Instantiate(nullptr);
	for (auto& rr : PrefabState::refresolvers.top()) {
		rr();
	}
	PrefabState::refresolvers.pop();
	PrefabState::activeBaseObjs.pop();
	//PrefabState::prefabStack.pop_back();
	_SceneObject::PrefabInfo info;

	info.prefab = get_shared<_Prefab>();
	info.ids = { res->id() };
	int i = 1;
	const std::function<void(const std::vector<SceneObject>&)> doadd
		= [&](const std::vector<SceneObject>& oo) {
		for (auto& o : oo) {
			info.ids.push_back(o->id());
			auto info2 = o->prefabInfo();
			if (!info2.prefab) { //this object is spawned by this prefab
				info2.prefab = info.prefab;
				info2.head = res;
				info2.id = i++;
			}
			else if (!info2.head && !info2.id) { //we register the head of a spawned object
				info2.id = i++;
			}
			o->prefabInfo(info2);
			doadd(o->children());
		}
	};
	doadd(res->children());
	res->prefabInfo(info);
	return res;
}

CE_END_NAMESPACE