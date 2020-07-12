#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_AnimGrapher::Ops {

public:
	static void Reg();

    CE_DEF_CALLBACK(StateAddNew);
};

CE_END_ED_NAMESPACE