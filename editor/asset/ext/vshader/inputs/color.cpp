#include "asset/ext/visual_shader.hpp"

CE_BEGIN_ED_NAMESPACE

VSN_Input_Color::VSN_Input_Color() : _VShaderNode("Color") {
	outputs.push_back(ConnO("color", ConnO::Type::Color));
}

void VSN_Input_Color::ParseOptions(const JsonObject& data) {

}

CE_END_ED_NAMESPACE