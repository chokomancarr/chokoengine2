#include "ce2/modules/pe/player_debug.hpp"

CE_BEGIN_MOD_PE_NAMESPACE

namespace {
    SharedMemory<PDSyncBaseSt> baseMem;

    SharedMemory<char> pixelsMem;

    bool check_clear_flag(uint32_t i) {
        if (!!(baseMem->status_flags & i)) {
            baseMem->status_flags &= ~i;
            return true;
        }
        else return false;
    }
}

void PDSyncer::Init() {
    baseMem.open(MemNms::base);
    //
    //pixelsMem = SharedMemory<char>("CE_APP_OUTPUT_PIXELS", pxls.size());
}

void PDSyncer::SyncFrame() {
    WaitForFlag(PDSyncFlags::EDITOR_SYNCED, true);

    if (check_clear_flag(PDSyncFlags::RESIZE)) {
        Display::Resize(baseMem->screen_width, baseMem->screen_height, false);
    }

    baseMem->screen_width = Display::width();
    baseMem->screen_height = Display::height();

    volatile auto flags = baseMem->status_flags;
    baseMem->status_flags = (flags & ~PDSyncFlags::EDITOR_SYNCED) | PDSyncFlags::APP_SYNCED;

    msync((void*)baseMem.data(), baseMem.length(), MS_SYNC);
}

void PDSyncer::WriteScreenOutput(const std::vector<char>& pxls) {
    std::memcpy((void*)pixelsMem.data(), pxls.data(), pxls.size());
}

void PDSyncer::WaitForFlag(uint32_t f, bool b) {
    volatile auto val = baseMem.data();
    while (!(val->status_flags & f) == b);
}

volatile PDSyncBaseSt& PDSyncer::GetBaseSt() {
    return baseMem[0];
}

CE_END_MOD_PE_NAMESPACE