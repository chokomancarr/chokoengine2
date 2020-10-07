#include "visual_shader.hpp"

CE_BEGIN_ED_NAMESPACE

VShaderNode _VShaderNode::FromSig(const std::string& name) {
#define SW(nm, cls, ...) if (name == #nm) return std::make_shared<cls>(__VA_ARGS__);
	SW(color_input, VSN_Input_Color)
	else SW(surface_output, VSN_Output_Surface)
	else CE_ABORT();
};

void _VShaderNode::Parse(const JsonObject& data, VShader& shad) {
	for (auto& d : data.group) {
		if (d.key.string == "options") {
			LoadOptions(d.value);
		}
		else if (d.key.string == "inputs") {
			for (auto& v : d.value.group) {
				for (auto& c : inputs) {
					if (v.key.string == c.name) {
						const auto& tn = v.value.Get("target");
						if (tn.string == "none") {
							switch (c.type) {
							case ConnI::Type::Scalar:
								c.value_f = v.value.Get("value").ToFloat();
								break;
							default:
								break;
							}
						}
						else {
							auto& tar = shad->nodes[tn.ToFloat()];
							c.target.nd = tar;
							const auto& cn = v.value.Get("conn").string;
							c.target.i = 0;
							for (auto& o : tar->outputs) {
								if (o.name == cn) {
									break;
								}
								c.target.i++;
							}
						}
						break;
					}
				}
			}
		}
		else if (d.key.string == "position") {
			pos.x = d.value.list[0].ToFloat();
			pos.y = d.value.list[1].ToFloat();
		}
	}
}

CE_END_ED_NAMESPACE