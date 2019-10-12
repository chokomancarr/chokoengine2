#pragma once
#include "chokoeditor.hpp"
#include "vshader_node.hpp"

CE_BEGIN_ED_NAMESPACE

/* Surface Shader generator using graphs
 */

class _VShader : public _Shader {
public:
	std::vector<VShaderNode> nodes;

	void Compile();
};

CE_END_ED_NAMESPACE