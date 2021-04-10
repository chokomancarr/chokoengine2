#include "chokoeditor.hpp"
#include "glsl/wcol.h"

CE_BEGIN_ED_NAMESPACE

Shader EW_S_Resources::unlitShad;
Material EW_S_Resources::unlitMat;

void EW_S_Resources::Init() {
	unlitShad = Shader::New(glsl::wColVert, glsl::wColFrag);
	unlitShad->AddUniform("MVP", ShaderVariableType::Matrix);
	unlitShad->AddUniform("col", ShaderVariableType::Color);

	(unlitMat = Material::New())->shader(unlitShad);
}

CE_END_ED_NAMESPACE