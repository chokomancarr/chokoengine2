#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_PrefabObj::_PrefabObj(const SceneObject& o, bool lnk) {
	name = "object";
	CE_PR_ADDV(name, o->name());
	const auto& tr = o->transform();
	CE_PR_ADDV(position, tr->localPosition());
	CE_PR_ADDV(rotation, *(Vec4*)&tr->localRotation());
	CE_PR_ADDV(scale, tr->localScale());
	auto& cc = o->components();
	if (cc.size() > 0) {
		auto& comps = CE_PR_ADDGROUP(components);
		for (auto& c : cc) {
			comps.push_back(PrefabComp_New(c));
		}
	}
	auto& cch = o->children();
	if (cch.size() > 0) {
		auto& childs = CE_PR_ADDGROUP(children);
		for (auto& c : cch) {
			if (lnk) {
				if (!!c->prefab()) {
					childs.push_back(PrefabLink_New(c));
					continue;
				}
			}
			childs.push_back(PrefabObj_New(c, lnk));
		}
	}
}

_PrefabObj::_PrefabObj(const JsonObject& json) : _PrefabObjBase(json) {}

SceneObject _PrefabObj::Instantiate(const SceneObject& pr) const {
	SceneObject res = SceneObject::New(
		CE_PR_GET(name, std::string),
		CE_PR_GET(position, Vec3),
		CE_PR_GET(rotation, Quat),
		CE_PR_GET(scale, Vec3)
	);
	const auto& comps = CE_PR_GETI(components).value.objgroup;
	for (auto& c : comps) {
		c->Instantiate(res);
	}
	const auto& chlds = CE_PR_GETI(children).value.objgroup;
	for (auto& c : chlds) {
		c->Instantiate(res);
	}

	res->parent(pr);

	return res;
}

CE_END_NAMESPACE