#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
    DummyScript _target;
}

void _DummyScriptLoader::Init() {
    instance = std::make_shared<_DummyScriptLoader>();
}

CE_PL_NS ScriptLoaderBase _DummyScriptLoader::GetLoaderOf(const std::string& sig) {
    std::dynamic_pointer_cast<_DummyScriptLoader>(instance)->_info = EAssetList::GetScr(sig);
    return instance;
}

std::string _DummyScriptLoader::sig() {
    return _target->info()->sig;
}

Component _DummyScriptLoader::Instantiate() {
    _target = DummyScript::New(_info);
    return _target;
}

void _DummyScriptLoader::activeTarget(const Component& c) {
    _target = (DummyScript)c;
}

std::vector<ScriptVar> _DummyScriptLoader::GetVarsOf(const std::string& sig) {
    auto& info = (sig.empty()) ? _target->info()
        : EAssetList::GetScr(sig);
    return info->vars;
}

namespace {
    ScriptVal& getslot(const std::string& nm) {
        auto& vs = _target->vals;
        auto it = std::find_if(vs.begin(), vs.end(), [&nm](const ScriptVal& v) {
            return v.var.name == nm;
        });
        return *it;
    }
}

#define IMPL_GET(tp) tp _DummyScriptLoader::get_ ## tp(const std::string& nm)
#define IMPL_SET(tp) void _DummyScriptLoader::set_ ## tp(const std::string& nm, const tp& vl)
#define GETV auto& v = getslot(nm);
#define IMPL_GETSET(tp, i)\
    IMPL_GET(tp) {\
        GETV\
        return v.val_ ## i;\
    }\
    IMPL_SET(tp) {\
        GETV\
        v.val_ ## i = vl;\
    }

IMPL_GETSET(int, i)
IMPL_GETSET(float, f)
IMPL_GETSET(Vec2, v2)
IMPL_GETSET(Vec3, v3)
IMPL_GETSET(Vec4, v4)

IMPL_GET(Quat) { GETV
    return Quat::identity();
}

IMPL_SET(Quat) { GETV
    
}

IMPL_GETSET(Asset, asset)
IMPL_GETSET(SceneObject, obj)

CE_END_PL_NAMESPACE