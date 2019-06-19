#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_Inspector : public EWindow {

protected:
	void DrawMenu() override;

public:
	bool Init() override;
};

CE_END_ED_NAMESPACE