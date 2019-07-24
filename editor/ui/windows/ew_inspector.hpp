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

	bool Init() override;
};

CE_END_ED_NAMESPACE

#include "inspector/sceneobject.hpp"