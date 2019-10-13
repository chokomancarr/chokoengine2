#include "asset/ext/visual_shader.hpp"

CE_BEGIN_ED_NAMESPACE

VSN_Output_Surface::VSN_Output_Surface() : _VShaderNode("Surface Output") {
	inputs.push_back(ConnI("Diffuse", ConnI::Type::Color));
	inputs.push_back(ConnI("Metallic", ConnI::Type::Color));
	inputs.push_back(ConnI("Roughness", ConnI::Type::Color));
	inputs.push_back(ConnI("Normal", ConnI::Type::Vector));
	inputs.push_back(ConnI("Emission", ConnI::Type::Color));
}

void VSN_Output_Surface::LoadOptions(const JsonObject& data) {

}

JsonObject VSN_Output_Surface::SaveOptions() {
	return JsonObject();
}

CE_END_ED_NAMESPACE