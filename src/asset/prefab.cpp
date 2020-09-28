#include "prefab/prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_Prefab::_Prefab(const SceneObject& o, bool link) : _Asset(AssetType::Prefab) {
	PrefabState::activeBaseObjs.push(o);
	const auto& info = o->prefabInfo();
	if (link && !!info.prefab && !info.head) { //this object (and its children) is spawned
		_data = PrefabLink_New(o, o->parent().lock(), true);
	}
	else {
		_data = PrefabObj_New(o, o->parent().lock(), link, true, true);
	}
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

SceneObject _Prefab::Instantiate(_Sig2Ass fn) {
	PrefabState::sig2AssFn = fn;
	//PrefabState::prefabStack.push_back(get_shared<_Prefab>());
	PrefabState::activeBaseObjs.push(nullptr);
	PrefabState::ids_indirect.push({});
	PrefabState::refresolvers.push({});

	const auto& res = _data->Instantiate(nullptr);
	for (auto& rr : PrefabState::refresolvers.top()) {
		rr();
	}
	_SceneObject::PrefabInfo info;

	info.prefab = get_shared<_Prefab>();
	info.ids = { res->id() };
	info.ids_indirect = PrefabState::ids_indirect.top();
	int i = 1;
	int j = 0;
	const std::function<void(const std::vector<SceneObject>&)> doadd
		= [&](const std::vector<SceneObject>& oo) {
		for (auto& o : oo) {
			info.ids.emplace(o->id());
			auto info2 = o->prefabInfo();
			if (!info2.prefab) { //this object is spawned by this prefab
				info2.prefab = info.prefab;
				info2.head = res;
				if (!info.ids_indirect.count(o->id())) {
					info2.id = i++;
				}
				else {
					info2.id = (1 << 32) + j++;
				}
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

	PrefabState::refresolvers.pop();
	PrefabState::ids_indirect.pop();
	PrefabState::activeBaseObjs.pop();

	_UpdateObjs();

	return res;
}

namespace {
	PrefabObjBase& _Get(PrefabObjBase& data, size_t& a) {
		static PrefabObjBase invalid = nullptr;

		if (!a--) {
			return data;
		}
		else {
			auto& items = data->items;
			const auto& chlds = CE_PR_GETI(children);
			CE_PR_IFVALID(chlds) {
				for (auto& c : chlds->second.value.objgroup) {
					auto& res = _Get(c, a);
					if (res != invalid)
						return res;
				}
			}
			return invalid;
		}
	}
}

PrefabObjBase& _Prefab::GetPrefabObj(size_t id) {
	return _Get(_data, id);
}

namespace {
	typedef _Prefab::_ObjTreeBase _TreeBase;

	void _AddBranch(PrefabObjBase& data, _TreeBase& tree) {
		if (data->_type == _PrefabObjBase::Type::Object) {
			tree.obj = data.get();
			tree.name = _CE_PR_GET<std::string>(tree.obj, "name", "");
		}
		else {
			auto lnk = (_PrefabLink*)data.get();
			auto tar = lnk->GetTarget();
			tree = *tar->GetTree();

			for (auto& m : lnk->mods) {
				auto& tar = m->target.Seek<_TreeBase&>(tree,
					[](_TreeBase& t) {
						return t.name;
					},
					[](_TreeBase& t) {
						return t.children.size();
					},
					[](_TreeBase& t, size_t i) -> _TreeBase& {
						return t.children[i];
					});
				tar.mods.push_back(m.get());
			}
		}
		auto& items = data->items;
		const auto& chlds = CE_PR_GETI(children);
		CE_PR_IFVALID(chlds) {
			for (auto& c : chlds->second.value.objgroup) {
				tree.children.push_back({});
				_AddBranch(c, tree.children.back());
			}
		}
	}
}

std::unique_ptr<_TreeBase>& _Prefab::GetTree() {
	return _tree;
}

void _Prefab::_UpdateObjs() {
	_tree = std::unique_ptr<_ObjTreeBase>(new _ObjTreeBase());
	_AddBranch(_data, *_tree);
}

CE_END_NAMESPACE