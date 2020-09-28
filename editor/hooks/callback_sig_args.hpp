#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

#define CALLBACKSIGARG(sig, ...) {CallbackSig::sig, ECallbackArgs({\
	__VA_ARGS__\
})}
#define CALLBACKSIGARG_EMPTY(sig) {CallbackSig::sig, ECallbackArgs({})}


const std::unordered_map<CallbackSig, ECallbackArgs> CallbackSigArgs = {
	CALLBACKSIGARG_EMPTY(GLOBAL_OPEN),



	CALLBACKSIGARG(ASSET_REFRESH,
		ECallbackArg("sig", "")
	),
	CALLBACKSIGARG(ASSET_REIMPORT,
		ECallbackArg("sig", "")
	),
	CALLBACKSIGARG(ASSET_DELETE,
		ECallbackArg("sig", "")
	),


	CALLBACKSIGARG(ASSET_PREFAB_CREATE,
		ECallbackArg("obj", pObject()),
		ECallbackArg("path", "")
	),
	CALLBACKSIGARG(ASSET_PREFAB_SPAWN,
		ECallbackArg("obj", pObject())
	),


	CALLBACKSIGARG_EMPTY(ANIMGRAPH_SAVE),

	CALLBACKSIGARG_EMPTY(ANIMGRAPH_STATE_NEW),
	CALLBACKSIGARG(ANIMGRAPH_STATE_DELETE,
		ECallbackArg("id", -1)
	),
	CALLBACKSIGARG(ANIMGRAPH_SET_LINK_SOURCE,
		ECallbackArg("id", -1)
	)
};

#undef CALLBACKSIGARG

CE_END_ED_NAMESPACE