#pragma once
#include "ce2/modules/pe/player_debug.hpp"

CE_BEGIN_MOD_PE_NAMESPACE

/* Base structure for editor-app syncing
 */
struct PDSyncBaseSt {
    uint32_t status_flags;
    uint32_t screen_width;
    uint32_t screen_height;
    Input::State input_state;
};

namespace PDSyncFlags {
    const uint32_t APP_SYNCED = 1 << 0;
    const uint32_t EDITOR_SYNCED = 1 << 1;
    const uint32_t RESIZE = 1 << 2;
    const uint32_t PAUSE = 1 << 3;
	const uint32_t KILL = 1 << 31;
}

CE_END_MOD_PE_NAMESPACE