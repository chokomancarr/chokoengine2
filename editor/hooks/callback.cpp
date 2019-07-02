#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

ECallback::ECallback(std::nullptr_t) : func(nullptr) {}

ECallback::ECallback(ECallback::_Func f) : func(f) {}

bool ECallback::operator!() {
    return !func;
}

void ECallback::operator()(EWindow* const w, const ECallbackArgs& args) {
    if (!func)
        Debug::Error("Editor Callback", "Function is null!");
    else
        func(w, args);
}

CE_END_ED_NAMESPACE