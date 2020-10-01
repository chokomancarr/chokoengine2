#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_PrefabLink::_PrefabLink(const SceneObject& obj, const SceneObject& par, bool flnk, int depth)
		: _PrefabObjBase(Type::Link) {
	name = "prefab";
	tar = obj->prefabInfo().uids[depth].prefab.lock();

	CE_PR_ADDV(target, tar);
	CE_PR_ADDV(name, obj->name());
	const auto& tr = obj->transform();
	CE_PR_ADDV(position, tr->localPosition());
	CE_PR_ADDV(rotation, tr->localRotation());
	CE_PR_ADDV(scale, tr->localScale());
	if (obj->parent() != par) {
		CE_PR_ADDV(parent, Prefab_ObjRef(obj, par));
	}

	PrefabObjGroup chlds = {};

	const auto& cids = obj->prefabInfo().ids;
	
	const std::function<void(const SceneObject&, bool)> find = [&](const SceneObject& o, bool first) {
		const auto& info = o->prefabInfo();
		const auto id = o->id();

		/* we check if the object is spawned by any prefab other than the base object
		 * as prefabs may contain other prefabs as well
		 */
		if (!!cids.count(id)) { //this object is spawned with this prefab
			const auto& ptar = tar->GetTreeObj(info.uids[depth].id);
			auto mod = PrefabMod_New(ptar, o, obj);
			if (!mod->empty()) {
				mods.push_back(std::move(mod));
			}
			if (flnk) { //we are topmost, check for non-linked objects in children
				for (auto& c : o->children()) {
					find(c, false);
				}
			}
		}
		else if (!!info.uids.size() && !info.uids.back().id) { //spawn a prefab
			auto it = std::find_if(info.uids.begin(), info.uids.end(),
					[](const _SceneObject::PrefabInfo::UID& uid) {
				return !uid.id;
			});
			chlds.push_back(PrefabLink_New(o, obj, false, it - info.uids.begin()));
		}
		else { //spawn
			chlds.push_back(PrefabObj_New(o, obj, true, false, false));
		}
	};

	find(obj, true);
	if (chlds.size() > 0) {
		CE_PR_ADDV(children, std::move(chlds));
	}
}

_PrefabLink::_PrefabLink(const JsonObject& json) : _PrefabObjBase(Type::Link, json) {}

Prefab& _PrefabLink::GetTarget() {
	if (!tar) {
		tar = CE_PR_GETI(target)->second.value.assetref.Load<Prefab>(AssetType::Prefab);
	}
	return tar;
}

void _PrefabLink::LoadLinks() {
	GetTarget();
}

SceneObject _PrefabLink::Instantiate(const SceneObject& pr) {
	auto res = GetTarget()->Instantiate(PrefabState::sig2AssFn);

	res->name(_CE_PR_GET<std::string>(this, "name", res->name()));
	res->transform()->localPosition(CE_PR_GET(position, Vec3()));
	res->transform()->localRotation(CE_PR_GET(rotation, Quat::identity()));
	res->transform()->localScale(CE_PR_GET(scale, Vec3(1)));

	const auto& par = CE_PR_GETI(parent);
	CE_PR_IFVALID(par) {
		auto pr2 = par->second.value.scobjref.Seek(PrefabState::activeBaseObjs.top());
		if (!!pr2) {
			res->parent(pr2);
		}
		else {
			return nullptr;
		}
	}
	else {
		res->parent(pr);
	}

	const auto& chlds = CE_PR_GETI(children);
	CE_PR_IFVALID(chlds) {
		for (auto& c : chlds->second.value.objgroup) {
			c->Instantiate(res);
		}
	}

	for (auto& m : mods) {
		m->Instantiate(res);
	}

	return res;
}

CE_END_NAMESPACE