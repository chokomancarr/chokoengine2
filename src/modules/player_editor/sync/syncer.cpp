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

    constexpr uint64_t FLAG_TIMEOUT = 5000;
}

void PDSyncer::Init() {
    baseMem.open(MemNms::base);
}

void PDSyncer::Cleanup() {
	baseMem.close();
}

bool PDSyncer::SyncFrame() {
    WaitForFlag(PDSyncFlags::EDITOR_SYNCED, true);

	if (!!(baseMem->status_flags & PDSyncFlags::KILL)) {
		Debug::Message("Syncer", "Received kill signal! Exiting ...");
		volatile auto flags = baseMem->status_flags;
		baseMem->status_flags = (flags & ~PDSyncFlags::EDITOR_SYNCED) | PDSyncFlags::APP_SYNCED;
		return false;
	}

    if (Display::width() != baseMem->screen_width ||
        Display::height() != baseMem->screen_height) {
        Display::Resize(baseMem->screen_width, baseMem->screen_height, false);
    }

    baseMem->screen_width = Display::width();
    baseMem->screen_height = Display::height();

	PlayerDebug::inputState = (const Input::State&)baseMem->input_state;

    const volatile auto pxlsz = baseMem->screen_width * baseMem->screen_height * 4;
    if (pxlsz != pixelsMem.length()) {
        pixelsMem.open(MemNms::pixels, pxlsz);
        volatile auto flags = baseMem->status_flags;
        baseMem->status_flags = flags | PDSyncFlags::RESIZE;
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glReadPixels(0, 0, Display::width(), Display::height(), GL_RGBA, GL_UNSIGNED_BYTE, (void*)pixelsMem.data());

    volatile auto flags = baseMem->status_flags;
    baseMem->status_flags = (flags & ~PDSyncFlags::EDITOR_SYNCED) | PDSyncFlags::APP_SYNCED;

	return true;
}

void PDSyncer::WriteScreenOutput(const std::vector<char>& pxls) {
    std::memcpy((void*)pixelsMem.data(), pxls.data(), pxls.size());
}

void PDSyncer::WaitForFlag(uint32_t f, bool b) {
    static auto millis = Time::actualMillis();
    volatile auto val = baseMem.data();
    while (!(val->status_flags & f) == b) {
        if (Time::actualMillis() - millis > FLAG_TIMEOUT) {
            abort();
        }
    }
    millis = Time::actualMillis();
}

volatile PDSyncBaseSt& PDSyncer::GetBaseSt() {
    return baseMem[0];
}

CE_END_MOD_PE_NAMESPACE