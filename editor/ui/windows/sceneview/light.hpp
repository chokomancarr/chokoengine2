#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_S_Light {

public:
	static void Init();

	static void Draw(const Component& lht, const Mat4x4& p);
	static void DrawActive(const Component& lht, const Mat4x4& p);
};

CE_END_ED_NAMESPACE