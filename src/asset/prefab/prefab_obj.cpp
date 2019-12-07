#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_PrefabObj::_PrefabObj(const SceneObject& o, bool lnk, bool flnk) {
	name = "object";
	CE_PR_ADDV(name, o->name());
	const auto& tr = o->transform();
	CE_PR_ADDV(position, tr->localPosition());
	CE_PR_ADDV(rotation, tr->localRotation());
	CE_PR_ADDV(scale, tr->localScale());
	auto& cc = o->components();
	if (cc.size() > 0) {
		auto& comps = CE_PR_ADDGROUP(components);
		for (auto& c : cc) {
			comps.push_back(PrefabComp_New(c));
		}
	}
	const auto pl = o->prefabs().size();
	auto& cch = o->children();
	if (cch.size() > 0) {
		auto& childs = CE_PR_ADDGROUP(children);
		for (auto& c : cch) {
			if (lnk) {
				if (c->prefabs().size() > pl) {
					childs.push_back(PrefabLink_New(c, flnk));
					continue;
				}
			}
			childs.push_back(PrefabObj_New(c, lnk, flnk));
		}
	}
}

_PrefabObj::_PrefabObj(const JsonObject& json) : _PrefabObjBase(json) {}

SceneObject _PrefabObj::Instantiate(const SceneObject& pr) const {
	SceneObject res = SceneObject::New(
		_CE_PR_GET<std::string>(this, "name", "New Object"),
		CE_PR_GET(position, Vec3()),
		CE_PR_GET(rotation, Quat::identity()),
		CE_PR_GET(scale, Vec3(1))
	);

	std::vector<pPrefab> prefabs;
	if (!!pr) {
		prefabs = pr->prefabs();
	}
	prefabs.push_back(PrefabState::activePrefabs.top());
	res->prefabs(prefabs);

	const auto& comps = CE_PR_GETI(components);
	CE_PR_IFVALID(comps) {
		for (auto& c : comps->second.value.objgroup) {
			c->Instantiate(res);
		}
	}
	const auto& chlds = CE_PR_GETI(children);
	CE_PR_IFVALID(chlds) {
		for (auto& c : chlds->second.value.objgroup) {
			c->Instantiate(res);
		}
	}

	res->parent(pr);

	return res;
}

CE_END_NAMESPACE