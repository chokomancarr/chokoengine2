#pragma once
#include "asset/ext/visual_shader.hpp"

CE_BEGIN_ED_NAMESPACE

class VSN_Input_Color : public _VShaderNode {
	Color c;
public:
	VSN_Input_Color();

	void LoadOptions(const JsonObject& data) override;
	JsonObject SaveOptions() override;
};

CE_END_ED_NAMESPACE