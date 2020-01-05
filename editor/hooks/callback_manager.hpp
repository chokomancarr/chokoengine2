#pragma once
#include "chokoeditor.hpp"
#include "callback.hpp"

CE_BEGIN_ED_NAMESPACE

class ECallbackManager {
public:
	typedef std::function<void(const ECallbackArgs&)> CbFn;

	static void Init();
	static void Update();

    static void Register(CallbackSig, CbFn);
	static void Register(CallbackSig, std::function<void()>);
	static void Invoke(CallbackSig, const ECallbackArgs& args = ECallbackArgs());
};

CE_END_ED_NAMESPACE