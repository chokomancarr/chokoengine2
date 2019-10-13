#include "asset/ext/visual_shader.hpp"

CE_BEGIN_ED_NAMESPACE

VSN_Input_Color::VSN_Input_Color() : _VShaderNode("Color") {
	outputs.push_back(ConnO("color", ConnO::Type::Color));
}

void VSN_Input_Color::LoadOptions(const JsonObject& data) {
	c = data.Get("value").ToColor();
}

JsonObject VSN_Input_Color::SaveOptions() {
	return JsonObject({
		JsonPair(JsonObject("value"), JsonObject::FromColor(c))
	});
}

CE_END_ED_NAMESPACE