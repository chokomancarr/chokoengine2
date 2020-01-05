#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ECallbackCaller {
public:
	CallbackSig sig;
	ECallbackArgs args;

	ECallbackCaller(CallbackSig = CallbackSig::Undefined, ECallbackArgs = ECallbackArgs());

	void operator ()() const;
};

CE_END_ED_NAMESPACE