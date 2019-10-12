#pragma once
#include "asset/ext/visual_shader.hpp"

CE_BEGIN_ED_NAMESPACE

class VSN_Input_Color : public _VShaderNode {
	
public:
	VSN_Input_Color();

	void ParseOptions(const JsonObject& data) override;
};

CE_END_ED_NAMESPACE