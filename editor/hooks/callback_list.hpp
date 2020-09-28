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

	OBJECT_NEW,
	OBJECT_NEW_CHILD,
	OBJECT_DELETE,

	ASSET_REFRESH,
	ASSET_REIMPORT,
	ASSET_DELETE,

	ASSET_PREFAB_CREATE,
	ASSET_PREFAB_SPAWN,

	// --------- inspector -----------



	// ---------  3d view  -----------

	VIEW_TOGGLE_PERSPECTIVE,

	VIEW_FOCUS_OBJECT,
	VIEW_FOCUS_OBJECT_TREE,

	// -------  anim graph  ----------

	ANIMGRAPH_SAVE,

	ANIMGRAPH_STATE_NEW,
	ANIMGRAPH_STATE_DELETE,

	ANIMGRAPH_SET_LINK_SOURCE,
	ANIMGRAPH_SET_LINK_DEST,


	_COUNT,
	Undefined
};

CE_END_ED_NAMESPACE

#include "callback_list_str.hpp"