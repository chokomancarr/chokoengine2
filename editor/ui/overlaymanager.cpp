#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::vector<std::shared_ptr<EOverlay>> EOverlayManager::overlays;

void EOverlayManager::Init() {
	
}

void EOverlayManager::LoadOverlays() {
	
}

void EOverlayManager::Update() {
	for (auto& o : overlays) {
		o->Update();
	}
}

void EOverlayManager::Draw() {
	for (auto& o : overlays) {
		if (o->active)
			o->Draw();
	}
}

CE_END_ED_NAMESPACE
