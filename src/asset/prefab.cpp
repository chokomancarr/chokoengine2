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
	readonly = json.Get("readonly").ToBool();
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
					uid2.id = (size_t(1) << 32) + j++;
				}
				info2.uids.insert(info2.uids.begin(), uid2);
				o->prefabInfo(info2);
				doadd(o->children());
			}
		};
		doadd(res->children());

		info.uids.insert(info.uids.begin(), uid);
		res->prefabInfo(info);

		_UpdateObjs(res);
	}

	PrefabState::refresolvers.pop();
	PrefabState::ids_indirect.pop();
	PrefabState::activeBaseObjs.pop();

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

	void _AddBranchInd(_TreeBase& tree, const SceneObject& o) {
		static auto nullobj = PrefabObj_New(JsonObject());
		tree.obj = nullobj.get();
		tree.name = o->name();
		tree.indirect = true;
		const auto& chos = o->children();
		tree.children.reserve(chos.size());
		for (auto& c : chos) {
			tree.children.push_back({});
			_AddBranchInd(tree.children.back(), c);
		}
	}

	void _RmInd(_TreeBase& tr) {
		tr.indirect = false;
		for (auto& c : tr.children) {
			_RmInd(c);
		}
	}
	void _AddBranch(PrefabObjBase& data, _TreeBase& tree, const SceneObject& o) {
		if (data->_type == _PrefabObjBase::Type::Object) {
			tree.obj = data.get();
			tree.name = _CE_PR_GET<std::string>(tree.obj, "name", "");
			tree.indirect = false;
		}
		else {
			auto lnk = (_PrefabLink*)data.get();
			auto tar = lnk->GetTarget();
			tree = *tar->GetTree();
			_RmInd(tree);

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
		const auto& chos = o->children();
		auto chit = chos.begin();
		if (chos.size()) {
			const auto& chlds = CE_PR_GETI(children);
			CE_PR_IFVALID(chlds) {
				auto cpit = chlds->second.value.objgroup.begin();
				for (auto& po : chlds->second.value.objgroup) {
					while (chit < chos.end()) {
						//ignore all inherited children
						if ((*chit)->prefabInfo().uids.size() > 1) {
							chit++;
						}
						//add all indirect children without prefabobj
						else if ((*chit)->prefabInfo().uids[0].id >= (size_t(1) << 32)) {
							tree.children.push_back({});
							_AddBranchInd(tree.children.back(), *chit++);
						}
						else break;
					}
					//if has specified parent, spawn there
					const auto& items = po->items;
					const auto& par = CE_PR_GETI(parent);
					CE_PR_IFVALID(par) {
						auto& pr2 = par->second.value.scobjref.Seek<_TreeBase&>(tree,
							[](_TreeBase& t) {
							return t.name;
						},
							[](_TreeBase& t) {
							return t.children.size();
						},
							[](_TreeBase& t, size_t i) -> _TreeBase& {
							return t.children[i];
						});
						pr2.children.push_back({});
						_AddBranch(po, pr2.children.back(), par->second.value.scobjref.Seek(o));
					}
					else {
						tree.children.push_back({});
						_AddBranch(po, tree.children.back(), (*chit++));
					}
				}
			}
			while (chit < chos.end()) {
				if ((*chit)->prefabInfo().uids.size() == 1) {
					if ((*chit)->prefabInfo().uids[0].id >= (size_t(1) << 32)) {
						tree.children.push_back({});
						_AddBranchInd(tree.children.back(), *chit);
					}
				}
				chit++;
			}
		}
	}
}

std::unique_ptr<_TreeBase>& _Prefab::GetTree() {
	return _tree;
}

void _Prefab::Apply(const SceneObject& o, bool tree, size_t id) {
	if (id == size_t(-1)) {
		auto& uids = o->prefabInfo().uids;
		id = std::find_if(uids.begin(), uids.end(), [this](const _SceneObject::PrefabInfo::UID& uid) {
			return uid.prefab.lock().object().get() == this;
		})->id;
	}
	if (tree) {
		CE_NOT_IMPLEMENTED
	}

	auto& tar = GetTreeObj(id);

	CE_NOT_IMPLEMENTED
}

void _Prefab::Revert(const SceneObject&, size_t id) {

}

void _Prefab::_UpdateObjs(const SceneObject& o) {
	_tree = std::unique_ptr<_ObjTreeBase>(new _ObjTreeBase());
	_AddBranch(_data, *_tree, o);
}

CE_END_NAMESPACE