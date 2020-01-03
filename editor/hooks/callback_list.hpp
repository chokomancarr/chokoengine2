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

	// --------- inspector -----------




	_COUNT
};

const std::vector<std::string> CallbackSigStrs = {
	"GLOBAL_OPEN",
	"GLOBAL_SAVE",
	"GLOBAL_SAVE_AS",
	"GLOBAL_UNDO",
	"GLOBAL_REDO"
};

CE_END_ED_NAMESPACE