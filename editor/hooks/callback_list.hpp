#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

/* Callback signatures
 */
enum class CallbackSig {
	// --------- global keys ------------
	GLOBAL_OPEN,
	GLOBAL_SAVE,
	GLOBAL_SAVE_AS,
	GLOBAL_UNDO,
	GLOBAL_REDO,
	GLOBAL_PLAY,
	GLOBAL_PAUSE,
	GLOBAL_STOP,
	GLOBAL_BUILD,
	GLOBAL_BUILD_DEBUG,

	// --------- inspector -----------




	_COUNT,
	Undefined
};

CE_END_ED_NAMESPACE

#include "callback_list_str.hpp"