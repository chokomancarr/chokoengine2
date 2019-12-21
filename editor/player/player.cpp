#include "chokoeditor.hpp"
#include "ce2/modules/pe/player_debug.hpp"

using namespace CE_NS CE_MOD_PE_NS;

CE_BEGIN_ED_NAMESPACE

namespace {
    SharedMemory<PDSyncBaseSt> baseSt;
}

void EPlayer::Play() {
    //wait for compile to finish

    baseSt = SharedMemory<PDSyncBaseSt>(MemNms::base);

    baseSt->screen_width = 800;
    baseSt->screen_height = 600;

    baseSt->status_flags |= PDSyncFlags::WAIT_SYNC;

    Subprocess::ProcessInfo info;
    info.program = EProjectBuilder::dbgProgPath;
    info.workingDir = StrExt::ParentFd(info.program);
    info.wait = false;
    Subprocess::Run(info);
}

CE_END_ED_NAMESPACE
