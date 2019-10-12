#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_ShaderEditor : public EWindow {
protected:
	void DrawMenu() override;

public:
	EW_ShaderEditor();

	static VShader target;

	bool Init() override;
};

CE_END_ED_NAMESPACE