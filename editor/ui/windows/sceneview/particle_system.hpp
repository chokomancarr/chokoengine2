#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_S_ParSys {
public:
	static void Init();

	static void DrawActive(const Component& c, const Mat4x4& p);
};

CE_END_ED_NAMESPACE