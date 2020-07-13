#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ECallbackCaller {
public:
	CallbackSig sig;
	ECallbackArgs args;
	EWindow* wnd;

	ECallbackCaller(CallbackSig = CallbackSig::Undefined, ECallbackArgs = {}, EWindow* = nullptr);

	void operator ()() const;
};

CE_END_ED_NAMESPACE