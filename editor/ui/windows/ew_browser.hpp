#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_Browser : public EWindow {
protected:
	void DrawMenu() override {}

public:
	bool Init() override { return true; }

};

CE_END_ED_NAMESPACE