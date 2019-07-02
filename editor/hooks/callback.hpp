#pragma once
#include "chokoeditor.hpp"
#include "callback_args.hpp"

CE_BEGIN_ED_NAMESPACE

class ECallback {
    typedef void (*_Func)(EWindow* const, const ECallbackArgs&);

public:
    ECallback(std::nullptr_t);
    ECallback(_Func f);

    _Func func;

    bool operator !();
    void operator ()(EWindow* const w = nullptr, const ECallbackArgs& args = ECallbackArgs());
};

#define CE_DEF_CALLBACK(nm) static void nm(EWindow* const, const ECallbackArgs&);

#define CE_IMPL_CALLBACK(nm) void nm(EWindow* const window, const ECallbackArgs& args)

CE_END_ED_NAMESPACE