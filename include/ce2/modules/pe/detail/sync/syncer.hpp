#pragma once
#include "ce2/modules/pe/player_debug.hpp"
#include "sync_base_st.hpp"

CE_BEGIN_PL_NAMESPACE

class PDSyncer {
public:
    static void Init();

    static void WriteScreenOutput(const std::vector<char>&);

    static void WaitForFlag(uint32_t, bool);
};

CE_END_PL_NAMESPACE