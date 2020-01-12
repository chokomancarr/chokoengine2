#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

ECallbackArg::ECallbackArg(const std::string& nm, int i)
        : type(Type::Int), name(nm), i(i) {}

ECallbackArg::ECallbackArg(const std::string& nm, float f)
        : type(Type::Float), name(nm), f(f) {}

ECallbackArg::ECallbackArg(const std::string& nm, const std::string& s)
        : type(Type::String), name(nm), s(s) {}

ECallbackArg::ECallbackArg(const std::string& nm, const pObject& o)
        : type(Type::Object), name(nm), o(o) {}


ECallbackArgs::ECallbackArgs(const std::vector<ECallbackArg>& args)
        : _args(args) {}

void ECallbackArgs::Add(const ECallbackArg& arg) {
    _args.push_back(arg);
}

const ECallbackArg& ECallbackArgs::operator [](int i) const {
    return _args[i];
}

const ECallbackArg& ECallbackArgs::operator [](const std::string& s) const {
	for (auto& a : _args) {
		if (a.name == s) return a;
	}
	Debug::Error("Callback", "Callback requires non-provided argument \"" + s + "\"!");
	static ECallbackArg null(s, 0);
	return null;
}

CE_END_ED_NAMESPACE