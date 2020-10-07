#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_S_Resources {
public:
	static Shader unlitShad;
	static Material unlitMat;

	static void Init();
};

CE_END_ED_NAMESPACE