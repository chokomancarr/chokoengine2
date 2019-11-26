#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EPreview {
public:
	static void Init();

	static Texture GetMaterial(const Material&);
};

CE_END_ED_NAMESPACE