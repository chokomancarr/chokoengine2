#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ECallbackManager {
public:
	typedef std::function<void(const ECallbackArgs&)> CbFn;

    static void Register(CallbackSig, CbFn);
    static void Invoke(CallbackSig, const ECallbackArgs&);
};

CE_END_ED_NAMESPACE