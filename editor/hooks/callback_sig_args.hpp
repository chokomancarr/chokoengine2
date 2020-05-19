#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

#define CALLBACKSIGARG(sig, ...) {CallbackSig::sig, ECallbackArgs({\
	__VA_ARGS__\
})}

const std::unordered_map<CallbackSig, ECallbackArgs> CallbackSigArgs = {
	CALLBACKSIGARG(GLOBAL_OPEN,
		//no args
	),



	CALLBACKSIGARG(ASSET_REFRESH,
		ECallbackArg("sig", "")
	),
	CALLBACKSIGARG(ASSET_REIMPORT,
		ECallbackArg("sig", "")
	),
	CALLBACKSIGARG(ASSET_DELETE,
		ECallbackArg("sig", "")
	),


	CALLBACKSIGARG(PREFAB_SPAWN,
		ECallbackArg("sig", "")
	),

};

#undef CALLBACKSIGARG

CE_END_ED_NAMESPACE