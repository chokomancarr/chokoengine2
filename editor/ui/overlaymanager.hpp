#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EOverlay;

class EOverlayManager {
public:
	static std::vector<std::shared_ptr<EOverlay>> overlays;

	static void Init();
	static void LoadOverlays();
	static void Update();
	static void Draw();
};

CE_END_ED_NAMESPACE

#include "overlay.hpp"