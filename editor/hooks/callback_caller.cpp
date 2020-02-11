#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

ECallbackCaller::ECallbackCaller(CallbackSig sig, ECallbackArgs args) :
		sig(sig), args(args) {}

void ECallbackCaller::operator ()() const {
	ECallbackManager::Invoke(sig, args);
}

CE_END_ED_NAMESPACE