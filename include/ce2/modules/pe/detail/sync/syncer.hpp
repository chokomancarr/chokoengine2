#pragma once
#include "ce2/modules/pe/player_debug.hpp"
#include "sync_base_st.hpp"

CE_BEGIN_MOD_PE_NAMESPACE

namespace MemNms {
    const std::string base = "CE_APP_BASE";
    const std::string pixels = "CE_APP_OUTPUT_PIXELS";
}

class PDSyncer {
public:
    static void Init();

    static void WriteScreenOutput(const std::vector<char>&);

    static void WaitForFlag(uint32_t, bool);

    static PDSyncBaseSt GetBaseSt();
};

CE_END_MOD_PE_NAMESPACE