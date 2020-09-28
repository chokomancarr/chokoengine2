#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_PrefabObj::_PrefabObj(const SceneObject& o, const SceneObject& p, bool lnk, bool flnk, bool first)
		: _PrefabObjBase(Type::Object) {
	name = "object";
	CE_PR_ADDV(name, o->name());
	if (!first) {
		const auto& tr = o->transform();
		CE_PR_ADDV(position, tr->localPosition());
		CE_PR_ADDV(rotation, tr->localRotation());
		CE_PR_ADDV(scale, tr->localScale());
		const auto& p2 = o->parent().lock();
		if (p2 != p) {
			auto oref = Prefab_ObjRef(p2, p);
			oref.path.insert(oref.path.begin(), std::make_pair(p->name(), 0));
			CE_PR_ADDV(parent, std::move(oref));
		}
	}

	const auto& cc = o->components();
	if (cc.size() > 0) {
		auto& comps = CE_PR_ADDGROUP(components);
		for (auto& c : cc) {
			comps.push_back(PrefabComp_New(c));
		}
	}

	const auto& prb = o->prefabInfo().prefab;
	const auto& cch = o->children();
	if (cch.size() > 0) {
		auto& childs = CE_PR_ADDGROUP(children);
		for (auto& c : cch) {
			if (lnk) { //retain links
				const auto& info = c->prefabInfo();
				if (!!info.prefab && !info.head) { //this object (and its children) is spawned
					childs.push_back(PrefabLink_New(c, o, flnk));
					continue;
				}
			}
			childs.push_back(PrefabObj_New(c, o, lnk, flnk, false));
		}
	}
}

_PrefabObj::_PrefabObj(const JsonObject& json) : _PrefabObjBase(Type::Object, json) {}

SceneObject _PrefabObj::Instantiate(const SceneObject& pr) {
	SceneObject res = SceneObject::New(
		_CE_PR_GET<std::string>(this, "name", "New Object"),
		CE_PR_GET(position, Vec3()),
		CE_PR_GET(rotation, Quat::identity()),
		CE_PR_GET(scale, Vec3(1))
	);

	const bool istop = !PrefabState::activeBaseObjs.top();

	if (istop) PrefabState::activeBaseObjs.top() = res;

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

	//res->prefabs({PrefabState::prefabStack.back()});

	const auto& comps = CE_PR_GETI(components);
	CE_PR_IFVALID(comps) {
		for (auto& c : comps->second.value.objgroup) {
			c->Instantiate(res);
		}
	}
	
	//if any components added children, add them to indirect ids
	const std::function<void(const std::vector<SceneObject>&)> doadd
		= [&](const std::vector<SceneObject>& oo) {
		for (auto& o : oo) {
			PrefabState::ids_indirect.top().emplace(o->id());
			doadd(o->children());
		}
	};
	doadd(res->children());

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