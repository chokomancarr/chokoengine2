#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_PrefabLink::_PrefabLink(const SceneObject& obj, const SceneObject& par, bool flnk)
		: tar(obj->prefabInfo().prefab.lock()) {
	name = "prefab";

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
	
	/* if flnk, look for child objects that are not a part of sub-prefabs
	 * in that case, the first parent prefab should be us
	 */
	const std::function<void(const SceneObject&)> find = [&](const SceneObject& o) {
		const auto& info = o->prefabInfo();
		const auto id = o->id();
		/* we check if the object is spawned by any prefab other than the base object
		 * as prefabs may contain other prefabs as well
		 */
		if (std::find_if(cids.begin(), cids.end(), [id](ChokoEngine::objectid i) {
			return i == id;
		}) != cids.end()) { //this object is spawned with this prefab
			if (flnk) { //we are topmost, check for non-linked objects in children
				for (auto& c : o->children()) {
					find(c);
				}
			}
		}
		else if (!!info.prefab) { //spawn a prefab
			if (!!info.head) {
				Debug::Error("Prefab", "encountered non-head non-masked object in scan!");
			}
			chlds.push_back(PrefabLink_New(o, obj, false));
		}
		else { //spawn
			chlds.push_back(PrefabObj_New(o, obj, true, false, false));
		}
	};

	find(obj);
	if (chlds.size() > 0) {
		CE_PR_ADDV(children, std::move(chlds));
	}
}

_PrefabLink::_PrefabLink(const JsonObject& json) : _PrefabObjBase(json) {}

SceneObject _PrefabLink::Instantiate(const SceneObject& pr) const {
	auto prb = CE_PR_GETI(target)->second.value.assetref.Load<Prefab>(AssetType::Prefab);
	auto res = prb->Instantiate(PrefabState::sig2AssFn);

	res->name(_CE_PR_GET<std::string>(this, "name", res->name()));
	res->transform()->localPosition(CE_PR_GET(position, Vec3()));
	res->transform()->localRotation(CE_PR_GET(rotation, Quat::identity()));
	res->transform()->localScale(CE_PR_GET(scale, Vec3(1)));

	_SceneObject::PrefabInfo info;
	info.prefab = prb;
	info.head = res;
	const std::function<void(const std::vector<SceneObject>&)> doadd
			= [&](const std::vector<SceneObject>& oo) {
		for (auto& o : oo) {
			info.ids.push_back(o->id());
			auto info2 = o->prefabInfo();
			if (!info2.prefab) {
				info2.prefab = prb;
				info2.head = res;

			}
			doadd(o->children());
		}
	};
	doadd(res->children());

	const auto& par = CE_PR_GETI(parent);
	CE_PR_IFVALID(par) {
		auto pr2 = par->second.value.scobjref.Seek(PrefabState::activeBaseObjs.top()->children());
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

	return res;
}

CE_END_NAMESPACE