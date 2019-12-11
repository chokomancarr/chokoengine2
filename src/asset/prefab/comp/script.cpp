#include "chokoplayer.hpp"
#include "../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

typedef Player::_ScriptLoaderBase Loader;
typedef Player::ScriptVarEntry Var;

#define ADDV(tp, vr)\
    case Var::Type::tp: {\
        auto r = PrefabItem(Loader::instance->get_ ## vr(v.name));\
        r.name = v.name;\
        grp.push_back(std::move(r));\
        break;\
    }

CE_PR_IMPL_COMP(Script) {
    Loader::instance->activeTarget(c);
    CE_PR_ADDV(script, Loader::instance->sig());
    auto vars = Loader::instance->GetVarsOf("");
    CE_PR_ADDV(variables, PrefabItem::Type::ItemGroup);
    auto& grp = items.back().second.value.group;
    for (auto& v : vars) {
        switch (v.type) {
            ADDV(Float, float)
            ADDV(Int, int)
            ADDV(Vec2, Vec2)
            ADDV(Vec3, Vec3)
            ADDV(Vec4, Vec4)
            ADDV(Quat, Quat)
        default:
            break;
        }
    }
}

#define GETV(_k, _v) case PrefabItem::Type::_k:\
    loader->set_ ## _v(v.name, v.Get<_v>());\
    break;

CE_PR_IMPL_COMP_INST(Script) {
    auto tar = CE_PR_GETI(script);
    auto loader = Loader::instance->GetLoaderOf(
        tar->second.value.s);
	auto c = loader->Instantiate();
    o->_components.push_back(c);
    c->_object = o;
    const auto& vars = CE_PR_GETI(variables)->second.value.group;
    for (auto& v : vars) {
        switch (v.type) {
            GETV(Float, float)
            GETV(Int, int)
            GETV(Vec2, Vec2)
            GETV(Vec3, Vec3)
            GETV(Vec4, Vec4)
            GETV(Quat, Quat)
            GETV(Asset, Asset)
        default:
            CE_NOT_IMPLEMENTED
        }
    }
}

CE_END_NAMESPACE
