/*
 * Generated by the preprocessor script
 * DO NOT EDIT
 */
#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

const std::unordered_map<CallbackSig, std::string> CallbackSigStr = {
	{ CallbackSig::GLOBAL_NEW, "GLOBAL_NEW" },
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
	{ CallbackSig::OBJECT_PREFAB_APPLY_SINGLE, "OBJECT_PREFAB_APPLY_SINGLE" },
	{ CallbackSig::OBJECT_PREFAB_APPLY_TREE, "OBJECT_PREFAB_APPLY_TREE" },
	{ CallbackSig::OBJECT_PREFAB_RESET, "OBJECT_PREFAB_RESET" },
	{ CallbackSig::ASSET_REFRESH, "ASSET_REFRESH" },
	{ CallbackSig::ASSET_REIMPORT, "ASSET_REIMPORT" },
	{ CallbackSig::ASSET_DELETE, "ASSET_DELETE" },
	{ CallbackSig::ASSET_PREFAB_CREATE, "ASSET_PREFAB_CREATE" },
	{ CallbackSig::ASSET_PREFAB_SPAWN, "ASSET_PREFAB_SPAWN" },
	{ CallbackSig::VIEW_TOGGLE_PERSPECTIVE, "VIEW_TOGGLE_PERSPECTIVE" },
	{ CallbackSig::VIEW_FOCUS_OBJECT, "VIEW_FOCUS_OBJECT" },
	{ CallbackSig::VIEW_FOCUS_OBJECT_TREE, "VIEW_FOCUS_OBJECT_TREE" },
	{ CallbackSig::ANIMGRAPH_SAVE, "ANIMGRAPH_SAVE" },
	{ CallbackSig::ANIMGRAPH_STATE_NEW, "ANIMGRAPH_STATE_NEW" },
	{ CallbackSig::ANIMGRAPH_STATE_DELETE, "ANIMGRAPH_STATE_DELETE" },
	{ CallbackSig::ANIMGRAPH_SET_LINK_SOURCE, "ANIMGRAPH_SET_LINK_SOURCE" },
	{ CallbackSig::ANIMGRAPH_SET_LINK_DEST, "ANIMGRAPH_SET_LINK_DEST" },
	{ CallbackSig::_COUNT, "_COUNT" },
	{ CallbackSig::Undefined, "Undefined" }
};

CE_END_ED_NAMESPACE
