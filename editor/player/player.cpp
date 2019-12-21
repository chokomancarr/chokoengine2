#include "chokoeditor.hpp"
#include "ce2/modules/pe/player_debug.hpp"

using namespace CE_NS CE_MOD_PE_NS;

CE_BEGIN_ED_NAMESPACE

namespace {
    SharedMemory<PDSyncBaseSt> baseMem;

    void WaitForFlag(uint32_t f, bool b) {
        volatile auto val = baseMem.data();
        while (!(val->status_flags & f) == b);
    }
}

Int2 EPlayer::targetReso;

void EPlayer::Play() {
    //wait for compile to finish

    baseMem.open(MemNms::base);
    
    std::memset((void*)baseMem.data(), 0, sizeof(PDSyncBaseSt));

    baseMem->screen_width = targetReso.x;
    baseMem->screen_height = targetReso.y;

    baseMem->status_flags |= PDSyncFlags::EDITOR_SYNCED;

    Debug::Message("Player", "Starting \"" + EProjectBuilder::dbgProgPath + "\" ...");

    Subprocess::ProcessInfo info;
    info.program = EProjectBuilder::dbgProgPath;
    info.workingDir = StrExt::ParentFd(info.program);
    info.wait = false;
    Subprocess::Run(info);
}

void EPlayer::Sync() {
    WaitForFlag(PDSyncFlags::APP_SYNCED, true);



    volatile auto flags = baseMem->status_flags;
    baseMem->status_flags = (flags & ~PDSyncFlags::APP_SYNCED) | PDSyncFlags::EDITOR_SYNCED;

    msync((void*)baseMem.data(), baseMem.length(), MS_SYNC);
}

CE_END_ED_NAMESPACE
