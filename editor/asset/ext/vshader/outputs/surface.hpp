#pragma once
#include "asset/ext/visual_shader.hpp"

CE_BEGIN_ED_NAMESPACE

class VSN_Output_Surface : public _VShaderNode {
	
public:
	VSN_Output_Surface();

	void ParseOptions(const JsonObject& data) override;
};

CE_END_ED_NAMESPACE