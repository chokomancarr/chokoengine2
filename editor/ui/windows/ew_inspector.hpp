#pragma once
#include "chokoeditor.hpp"
#include "inspector/status/status_base.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_Inspector : public EWindow {
	Shader highlightShad;

protected:

	void DrawMenu() override;

public:
	static EW_I_Status::UMap statuses;

	static std::function<void(const Rect&)> customDrawer;

	EW_Inspector();

	bool Init() override;
};

CE_END_ED_NAMESPACE

#include "inspector/asset.hpp"
#include "inspector/sceneobject.hpp"