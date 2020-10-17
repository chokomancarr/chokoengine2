#include "chokoplayer.hpp"
#include "../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

typedef Player::_ScriptLoaderBase Loader;
typedef Player::ScriptVarEntry Var;

#define LDGET(fn) Loader::instance->get_ ## fn(parentsig + v.name)

#define ADDV(tp, vr)\
    case Var::Type::tp: {\
        return PrefabItem(LDGET(vr), v.name);\
    }

#define GETV(_k, _v) case PrefabItem::Type::_k:\
    Loader::instance->set_ ## _v(parentsig + v.name, v.Get<_v>());\
    break;

namespace {
	PrefabItem GetPrbItem(const Var& v, const std::string& parentsig = "", bool isv = false) {
		if (v.is_vector && !isv) {
			const auto sz = LDGET(vecsize);
			auto r = PrefabItem(PrefabItem::Type::ItemGroup);
			r.is_array = true;
			r.name = v.name;
			for (size_t a = 0; a < sz; a++) {
				const auto sa = std::to_string(a);
				r.value.group.push_back(GetPrbItem(v, parentsig + sa + "!", true));
			}
			if (!!sz) {
				r.type = r.value.group[0].type;
			}
			return r;
		}
		switch (v.type) {
			ADDV(Float, float)
			ADDV(Int, int)
			ADDV(Vec2, Vec2)
			ADDV(Vec3, Vec3)
			ADDV(Vec4, Vec4)
			ADDV(Quat, Quat)
			ADDV(Asset, Asset)
		case Var::Type::SceneObject: {
			return PrefabItem(Prefab_ObjRef(LDGET(SceneObject), PrefabState::activeBaseObjs.top()), v.name);
		}
		case Var::Type::Component: {
			return PrefabItem(Prefab_CompRef(LDGET(Component), PrefabState::activeBaseObjs.top()), v.name);
		}
		case Var::Type::Class: {
			auto r = PrefabItem(PrefabItem::Type::ItemGroup);
			r.name = v.name;
			auto vars2 = Loader::instance->GetVarsOf("." + v.sub_class);
			const auto psg = parentsig + v.name + ".";
			for (auto& v2 : vars2) {
				r.value.group.push_back(GetPrbItem(v2, psg));
			}
			return r;
		}
		default:
			return PrefabItem("invalid switch");
		}
	}

	void SetPrbItem(const PrefabItem& v, const Component& c, const std::string& parentsig = "", bool isv = false) {
		if (v.is_array) {
			const auto n = v.value.group.size();
			Loader::instance->set_vecsize(parentsig + v.name, n);
			for (size_t a = 0; a < n; a++) {
				SetPrbItem(v.value.group[a], c, parentsig + std::to_string(a) + "!" + v.name, true);
			}
		}
		else {
			switch (v.type) {
				GETV(Float, float)
				GETV(Int, int)
				GETV(Vec2, Vec2)
				GETV(Vec3, Vec3)
				GETV(Vec4, Vec4)
				GETV(Quat, Quat)
				GETV(Asset, Asset)
			case PrefabItem::Type::SceneObject: {
				const auto nm = v.name;
				const auto oref = v.Get<Prefab_ObjRef>();
				PrefabState::refresolvers.top().push_back([c, nm, oref]() {
					Loader::instance->activeTarget(c);
					const auto tar = oref.Seek(PrefabState::activeBaseObjs.top());
					Loader::instance->set_SceneObject(nm, tar);
				});
				break;
			}
			case PrefabItem::Type::Component: {
				const auto nm = v.name;
				const auto cref = v.Get<Prefab_CompRef>();
				PrefabState::refresolvers.top().push_back([c, nm, cref]() {
					Loader::instance->activeTarget(c);
					const auto tar = cref.objref.Seek(PrefabState::activeBaseObjs.top());
					if (!tar) return;
					for (auto& c : tar->components()) {
						if (c->componentType == cref.type) {
							Loader::instance->set_Component(nm, c);
							return;
						}
					}
				});
				break;
			}
			case PrefabItem::Type::ItemGroup: {
				for (auto& v2 : v.value.group) {
					SetPrbItem(v2, c, parentsig + v.name + ".");
				}
				break;
			}
			default:
				CE_NOT_IMPLEMENTED
			}
		}
	}
}

CE_PR_IMPL_COMP(Script) {
    Loader::instance->activeTarget(c);
    CE_PR_ADDV(script, Loader::instance->sig());
    auto vars = Loader::instance->GetVarsOf("");
    CE_PR_ADDV(variables, PrefabItem::Type::ItemGroup);
    auto& grp = items.back().second.value.group;
    for (auto& v : vars) {
		grp.push_back(GetPrbItem(v));
    }
}

CE_PR_IMPL_COMP_INST(Script) {
    auto tar = CE_PR_GETI(script);
    auto c = Loader::instance
		->GetLoaderOf(tar->second.value.s)
		->Instantiate();
    o->_components.push_back(c);
    c->_object = o;
    
	ApplyScript((Script)c);
}

CE_PR_IMPL_COMP_APP(Script) {
	const auto& vars = CE_PR_GETI(variables)->second.value.group;
	for (auto& v : vars) {
		SetPrbItem(v, c);
	}
}

CE_END_NAMESPACE
