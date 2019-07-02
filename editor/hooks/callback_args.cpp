#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

ECallbackArg::ECallbackArg(const std::string& nm, int i)
        : name(nm), i(i) {}

ECallbackArg::ECallbackArg(const std::string& nm, float f)
        : name(nm), f(f) {}

ECallbackArg::ECallbackArg(const std::string& nm, const std::string& s)
        : name(nm), s(s) {}

ECallbackArg::ECallbackArg(const std::string& nm, const pObject& o)
        : name(nm), o(o) {}


ECallbackArgs::ECallbackArgs(const std::vector<ECallbackArg>& args)
        : _args(args) {}

void ECallbackArgs::Add(const ECallbackArg& arg) {
    _args.push_back(arg);
}

const ECallbackArg& ECallbackArgs::operator [](int i) const {
    return _args[i];
}

CE_END_ED_NAMESPACE