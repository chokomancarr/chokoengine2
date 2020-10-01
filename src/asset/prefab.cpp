#include "prefab/prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_Prefab::_Prefab(const SceneObject& o, bool link) : _Asset(AssetType::Prefab) {
	PrefabState::activeBaseObjs.push(o);
	const auto& info = o->prefabInfo();
	if (link && !!info.uids.size()) { //this object (and its children) is spawned
		if (!info.uids[0].id)
			_data = PrefabLink_New(o, o->parent().lock(), true, 0);
		else {
			Debug::Error("Prefab::Prefab", "cannot create prefab from body of another prefab!");
			return;
		}
	}
	else {
		_data = PrefabObj_New(o, o->parent().lock(), link, true, true);
	}
	PrefabState::activeBaseObjs.pop();
}

_Prefab::_Prefab(const JsonObject& json, _Sig2Prb fn) : _Asset(AssetType::Prefab) {
	PrefabState::sig2PrbFn = fn;
	const auto tp = json.group[0].key.string;
	if (tp == "object") {
		_data = PrefabObj_New(json.group[0].value);
	}
	else if (tp == "prefab") {
		_data = PrefabLink_New(json.group[0].value);
	}
	else {
		Debug::Warning("Prefab", "Cannot create from json: no object or prefab entry!");
		_deleted = true;
	}
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

	/* fill in prefab info so we can
	 * link back when serializing
	 */
	if (_assetSignature != "<instantiated>") {
		typedef _SceneObject::PrefabInfo::UID UID;
		auto info = res->prefabInfo();
		UID uid;
		uid.prefab = get_shared<_Prefab>();
		uid.id = 0;
		uid.objs = { res };
		info.ids = { res->id() };
		info.ids_indirect = PrefabState::ids_indirect.top();

		int i = 1;
		int j = 0;
		const std::function<void(const std::vector<SceneObject>&)> doadd
			= [&](const std::vector<SceneObject>& oo) {
			for (auto& o : oo) {
				info.ids.emplace(o->id());
				uid.objs.push_back(o);
				auto info2 = o->prefabInfo();
				UID uid2;
				uid2.prefab = uid.prefab;
				uid2.head = res;
				if (!info.ids_indirect.count(o->id())) {
					uid2.id = i++;
				}
				else {
					uid2.id = (1 << 32) + j++;
				}
				info2.uids.insert(info2.uids.begin(), uid2);
				o->prefabInfo(info2);
				doadd(o->children());
			}
		};
		doadd(res->children());

		info.uids.insert(info.uids.begin(), uid);
		res->prefabInfo(info);
	}

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

	_Prefab::_ObjTreeBase& _GetT(_Prefab::_ObjTreeBase& data, size_t& a) {
		static _Prefab::_ObjTreeBase invalid = {};

		if (!a--) {
			return data;
		}
		else {
			for (auto& c : data.children) {
				auto& res = _GetT(c, a);
				if (&res != &invalid)
					return res;
			}
			return invalid;
		}
	}
}

PrefabObjBase& _Prefab::GetPrefabObj(size_t id) {
	return _Get(_data, id);
}

_Prefab::_ObjTreeBase& _Prefab::GetTreeObj(size_t id) {
	return _GetT(*_tree, id);
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