#include "chokoeditor.hpp"
#include "ce2/modules/pe/player_debug.hpp"

using namespace CE_NS CE_MOD_PE_NS;

CE_BEGIN_ED_NAMESPACE

namespace {
    SharedMemory<PDSyncBaseSt> baseMem;

    SharedMemory<char> pixelsMem;

    void WaitForFlag(uint32_t f, bool b) {
        volatile auto val = baseMem.data();
        while (!(val->status_flags & f) == b);
    }

    bool check_clear_flag(uint32_t i) {
        if (!!(baseMem->status_flags & i)) {
            baseMem->status_flags &= ~i;
            return true;
        }
        else return false;
    }
}

bool EPlayer::playing = false;

Int2 EPlayer::targetReso;

Texture EPlayer::outputImage;

void EPlayer::Init() {
	ECallbackManager::Register(CallbackSig::GLOBAL_PLAY, CbFnFrom(Play));
	ECallbackManager::Register(CallbackSig::GLOBAL_STOP, CbFnFrom(Stop));
}

void EPlayer::Play() {
    while (EProjectBuilder::busy());

	ESceneManager::SaveAs(".ce/startup.scene", false);

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

    playing = true;
}

void EPlayer::Sync() {
    WaitForFlag(PDSyncFlags::APP_SYNCED, true);

    const auto wh4 = baseMem->screen_width * baseMem->screen_height * 4;
    if (!!wh4) {
        if (!outputImage || baseMem->screen_width != outputImage->width() ||
                baseMem->screen_height != outputImage->height()) {
            outputImage = Texture::New(baseMem->screen_width, baseMem->screen_height, false, TextureOptions());
        }
        
        if (check_clear_flag(PDSyncFlags::RESIZE)) {
            pixelsMem.open(MemNms::pixels, wh4);
        }

        if (!!pixelsMem) {
            std::vector<byte> pxls(wh4);
            std::memcpy(pxls.data(), (void*)pixelsMem.data(), wh4);
            outputImage->SetPixelsRaw(pxls);
        }
    }

    baseMem->screen_width = targetReso.x;
    baseMem->screen_height = targetReso.y;

    volatile auto flags = baseMem->status_flags;
    baseMem->status_flags = (flags & ~PDSyncFlags::APP_SYNCED) | PDSyncFlags::EDITOR_SYNCED;

    //msync((void*)baseMem.data(), baseMem.length(), MS_SYNC);
}

void EPlayer::Stop() {
    playing = false;
}

CE_END_ED_NAMESPACE
