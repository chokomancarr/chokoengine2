/*
 * Generated by the preprocessor script
 * DO NOT EDIT
 */
#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

const std::unordered_map<CallbackSig, std::string> CallbackSigStr = {
	{ CallbackSig::GLOBAL_OPEN, "GLOBAL_OPEN" },
	{ CallbackSig::GLOBAL_SAVE, "GLOBAL_SAVE" },
	{ CallbackSig::GLOBAL_SAVE_AS, "GLOBAL_SAVE_AS" },
	{ CallbackSig::GLOBAL_UNDO, "GLOBAL_UNDO" },
	{ CallbackSig::GLOBAL_REDO, "GLOBAL_REDO" },
	{ CallbackSig::GLOBAL_PLAY, "GLOBAL_PLAY" },
	{ CallbackSig::GLOBAL_PAUSE, "GLOBAL_PAUSE" },
	{ CallbackSig::GLOBAL_STOP, "GLOBAL_STOP" },
	{ CallbackSig::GLOBAL_BUILD, "GLOBAL_BUILD" },
	{ CallbackSig::GLOBAL_BUILD_DEBUG, "GLOBAL_BUILD_DEBUG" },
	{ CallbackSig::OBJECT_NEW, "OBJECT_NEW" },
	{ CallbackSig::OBJECT_NEW_CHILD, "OBJECT_NEW_CHILD" },
	{ CallbackSig::OBJECT_DELETE, "OBJECT_DELETE" },
	{ CallbackSig::ASSET_REFRESH, "ASSET_REFRESH" },
	{ CallbackSig::ASSET_REIMPORT, "ASSET_REIMPORT" },
	{ CallbackSig::ASSET_DELETE, "ASSET_DELETE" },
	{ CallbackSig::PREFAB_SPAWN, "PREFAB_SPAWN" },
	{ CallbackSig::PREFAB_APPLY, "PREFAB_APPLY" },
	{ CallbackSig::_COUNT, "_COUNT" },
	{ CallbackSig::Undefined, "Undefined" }
};

CE_END_ED_NAMESPACE
