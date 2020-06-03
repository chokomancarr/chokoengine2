#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_Logger : public EWindow {

protected:
	void DrawMenu() override;

public:

	static std::vector<EDebug::MessageObj> msgs;
};

CE_END_ED_NAMESPACE