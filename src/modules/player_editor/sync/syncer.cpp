#include "ce2/modules/pe/player_debug.hpp"

CE_BEGIN_PL_NAMESPACE

namespace {
    SharedMemory<PDSyncBaseSt> baseMem;

    SharedMemory<char> pixelsMem;
}

void PDSyncer::Init() {
    baseMem = SharedMemory<PDSyncBaseSt>("CE_APP_DEBUG_BASE");
    //
    //pixelsMem = SharedMemory<char>("CE_APP_OUTPUT_PIXELS", pxls.size());
}

void PDSyncer::WriteScreenOutput(const std::vector<char>& pxls) {
    std::memcpy((void*)pixelsMem.data(), pxls.data(), pxls.size());
}

void PDSyncer::WaitForFlag(uint32_t f, bool b) {
    for (;;) {
        uint32_t val = baseMem->status_flags & f;
        if (!val != b) return;
    }
}

CE_END_PL_NAMESPACE