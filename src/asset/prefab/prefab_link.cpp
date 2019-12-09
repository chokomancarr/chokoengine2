#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

_PrefabLink::_PrefabLink(const SceneObject& obj, const SceneObject& par, bool flnk)
		: tar(obj->prefab()) {
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
	
	/* if flnk, look for child objects that are not a part of sub-prefabs
	 * in that case, the first parent prefab should be us
	 */
	const std::function<void(const SceneObject&)> find = [&](const SceneObject& o) {
		const auto& pr = o->prefab();
		/* we check if the object is spawned by any prefab other than the base object
		 * as prefabs may contain other prefabs as well
		 */
		if (PrefabState::prefabStack.back() == pr) { //this object is with the base object
			if (flnk) { //only the topmost link writes extra objects
				chlds.push_back(PrefabObj_New(o, obj, true, false, false));
			}
		}
		else if (pr != tar) { //different prefab, spawn
			std::cout << pr->assetSignature() << "  :  " << tar->assetSignature() << std::endl;
			chlds.push_back(PrefabLink_New(o, obj, false));
		}
		else {
			for (auto& c : o->children()) {
				find(c);
			}
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
	/* Note: prefab instantiates will write to the prefab stack,
	 * so no need to write it here.
	 */
	res->name(_CE_PR_GET<std::string>(this, "name", res->name()));
	res->transform()->localPosition(CE_PR_GET(position, Vec3()));
	res->transform()->localRotation(CE_PR_GET(rotation, Quat::identity()));
	res->transform()->localScale(CE_PR_GET(scale, Vec3(1)));

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