#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

ECallbackCaller::ECallbackCaller(CallbackSig sig, ECallbackArgs args, EWindow* wnd) :
		sig(sig), args(args), wnd(wnd) {}

void ECallbackCaller::operator ()() const {
	ECallbackManager::Invoke(sig, args, wnd);
}

CE_END_ED_NAMESPACE