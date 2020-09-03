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
	auto ss = StrExt::Split(sig, '.');
    auto& info = (ss[0].empty()) ? _target->info()
        : EAssetList::GetScr(ss[0]);
	if (ss.size() < 2) {
		return info->vars;
	}
	else {
		return std::find_if(info->classes.begin(), info->classes.end(), [&ss](const Player::ScriptClassEntry& e) {
			return e.name == ss[1];
		})->vars;
	}
}

namespace {
    ScriptVal& getslot(const std::string& nm) {
		const auto& ss = StrExt::Split(nm, '.');
		ScriptVal* res = nullptr;
		for (auto s : ss) {
			auto& vs = (res) ? res->val_class.val : _target->vals;
			const auto arrc = s.find('!');
			int arri = -1;
			if (arrc != std::string::npos) {
				arri = std::stoi(s.substr(0, arrc));
				s = s.substr(arrc + 1);
			}
			auto it = std::find_if(vs.begin(), vs.end(), [&s](const ScriptVal& v) {
				return v.var.name == s;
			});
			if (it == vs.end()) {
				EDebug::Log("ScriptLoader", "failed to read slot \"" + nm + "\" for script " + _target->info()->className + "!");
				return ScriptVal::invalid;
			}
			res = (arri > -1) ? &(it->val_vec[arri]) : &(*it);
		}
        return *res;
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
		if (&v != &ScriptVal::invalid)\
	        v.val_ ## i = vl;\
    }

void _DummyScriptLoader::set_vecsize(const std::string& nm, const size_t n) {
	GETV;
	v.val_vec.resize(n);
}
size_t _DummyScriptLoader::get_vecsize(const std::string& nm) {
	GETV;
	return v.val_vec.size();
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