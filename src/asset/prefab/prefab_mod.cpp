#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

namespace {
	typedef std::vector<std::pair<std::string, PrefabItem>> PIpv;
	bool ChkEq(PrefabItem& me, const PrefabItem& tar);
	void RmSame(PrefabItemGroup& me, const PrefabItemGroup& tar, bool repl) { //we assume only values change
		auto n = (int)me.size();
		if ((int)tar.size() != n) {
			return;
		}
		for (auto a = n - 1; a >= 0; a--) {
			if (ChkEq(me[a], tar[a])) {
				if (repl) {
					me[a].is_default = true;
				}
				else {
					std::swap(me[a], me.back());
					me.pop_back();
				}
			}
		}
	}
	void RmSame(PIpv& me, const PIpv& tar) { //we assume only values change
		auto n = (int)me.size();
		for (auto a = n - 1; a >= 0; a--) {
			auto& m = me[a];
			auto it = std::find_if(tar.begin(), tar.end(), [&m](PIpv::const_reference t) {
				return t.first == m.first;
			});
			if (it == tar.end()) continue;
			if (ChkEq(m.second, it->second)) {
				std::swap(me[a], me.back());
				me.pop_back();
			}
		}
	}

#pragma optimize("", off)
	bool ChkEq(PrefabItem& me, const PrefabItem& tar) {
		if (me.is_array) {
			auto& grp = me.value.group;
			if (grp.size() == tar.value.group.size()) {//if same size, check each contents
				RmSame(grp, tar.value.group, true);
				return std::find_if(grp.begin(), grp.end(), [](const PrefabItem& g) {
					return !g.is_default;
				}) == grp.end();
			}
			else { //different size, write all
				return false;
			}
		}

#define CS(tp, vl) \
	case PrefabItem::Type::tp:\
		return me.value.vl == tar.value.vl;

		switch (me.type) {
			CS(Float, f);
			CS(Int, i);
			CS(Vec2, v2);
			CS(Vec3, v3);
			CS(Vec4, v4);
			CS(Quat, q);
			CS(String, s);

		case PrefabItem::Type::ItemGroup: {
			RmSame(me.value.group, tar.value.group, false);
			return me.value.group.empty();
		}
		case PrefabItem::Type::Asset: {
			return me.value.assetref.sig == tar.value.assetref.sig;
		}
		//case PrefabItem::Type::Component: {
		//	return me.value.compref.objref.path == tar.value.compref.objref.path;
		//}
		default:
			CE_NOT_IMPLEMENTED
		}
	}
}

#pragma optimize("", off)
_PrefabMod::_PrefabMod(const _PrefabObj* obj, const SceneObject& tar, const SceneObject& root) {
	target = Prefab_ObjRef(tar, root);
	const PIpv _emptyitems = {};
	auto& pitems = obj ? obj->items : _emptyitems;

	using itp = std::pair<std::string, PrefabItem>;
	auto cgsp = std::find_if(pitems.begin(), pitems.end(), [](const itp& i) {
		return i.first == "components";
	});

	static const PrefabObjGroup _null;
	const auto& tars = (cgsp == pitems.end()) ? _null : cgsp->second.value.objgroup;
	const size_t n = tars.size();
	std::unordered_set<size_t> regd = {};

	int i = 0;
	for (auto& c : tar->components()) { //assume components are unique
		auto cprb = PrefabComp_New(c);
		bool found = false;
		for (size_t a = 0; a < n; a++) {
			const auto ctar = (_PrefabComp*)tars[a].get();
			bool is_same_comp = false;
			if (ctar->type == c->componentType && (regd.find(a) == regd.end())) {
				if (ctar->type == ComponentType::Script) {
					is_same_comp = (ctar->items[0].second.value.s == cprb->items[0].second.value.s);
				}
				else {
					is_same_comp = true;
				}
			}
			if (is_same_comp) { //check for changes
				RmSame(cprb->items, ctar->items);

				if (!cprb->items.empty()) {
					mod_comps.push_back(std::move(cprb));
				}
				found = true;
				regd.emplace(a);
				break;
			}
		}
		if (!found) { //add this comp
			add_comps.push_back(std::make_pair(i, std::move(cprb)));
		}
		i++;
	}
}

_PrefabMod::_PrefabMod(const JsonObject& json) {
	target = Prefab_ObjRef(json.Get("target"));
	const auto& modcomps = json.Get("modcomps");
	for (auto& m : modcomps.group) {
		mod_comps.push_back(PrefabComp_New(m));
	}
	const auto& addcomps = json.Get("addcomps");
	for (auto& a : addcomps.group) {
		const int i = a.value.Get("__comp__index.Int").ToInt();
		add_comps.push_back(std::pair<int, PrefabComp>(i, nullptr));
		add_comps.back().second = PrefabComp_New(a);
	}
}

bool _PrefabMod::empty() const {
	return add_comps.empty() && mod_comps.empty();
}

JsonObject _PrefabMod::ToJson() const {
	JsonObject res(JsonObject::Type::Group);

	res.group.push_back(JsonPair(JsonObject("target"), target.ToJson()));

	if (!mod_comps.empty()) {
		JsonObject mc(JsonObject::Type::Group);
		for (auto& m : mod_comps) {
			mc.group.push_back(m->ToJson());
		}

		res.group.push_back(JsonPair(JsonObject("modcomps"), mc));
	}
	
	return res;
}

void _PrefabMod::Instantiate(const SceneObject& obj) const {
	auto& o = target.Seek(obj);
	std::unordered_set<size_t> regd = {};
	const auto n = o->components().size();

	for (auto& c : add_comps) {
		c.second->Instantiate(o);
	}
	for (auto& c : mod_comps) {
		for (size_t a = 0; a < n; a++) {
			auto& ct = o->components()[a];
			bool is_same_comp = false;
			if (c->type == ct->componentType && (regd.find(a) == regd.end())) {
				if (c->type == ComponentType::Script) {
					//is_same_comp = (c->items[0].second.value.s == );
					//????
				}
				else {
					is_same_comp = true;
				}
			}
			if (is_same_comp) {
				c->Apply(ct);

				regd.emplace(a);
				break;
			}
		}
	}
}

CE_END_NAMESPACE