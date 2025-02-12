#include "inc.hpp"

CE_BEGIN_MOD_AC_NAMESPACE

Shader LoadShader(const JsonObject& data) {
    std::string nm, vs, fs;
	ShaderQueue qu = ShaderQueue::Opaque;
    JsonObject vrs;
    for (auto& d : data.group) {
        if (d.key.string == "name") {
            nm = d.value.string;
        }
        if (d.key.string == "type") {
			if (d.value.string == "Transparent") qu = ShaderQueue::Transparent;
			if (d.value.string == "Overlay") qu = ShaderQueue::Overlay;
            else if (d.value.string != "Opaque") {
                Debug::Warning("Shader Asset Loader", "unknown \"type\" value: \"" + d.value.string + "\" (accepted values: \"Opaque\", \"Transparent\", \"Overlay\")!");
            }
        }
        if (d.key.string == "variables") {
            vrs = d.value;
        }
        else if (d.key.string == "vertex") {
            vs = d.value.string;
        }
        else if (d.key.string == "fragment") {
            fs = d.value.string;
        }
    }
    auto shd = Shader::New(vs, fs);
    shd->name(nm);
    shd->queue(qu);
    shd->RegisterStandardUniforms();
    for (auto v : vrs.group) {
        std::string vtp = v.value.string;
        if (v.value.type == JsonObject::Type::List) {
            vtp = v.value.list[0].string;

        }
#define CE_E_SHV(nm) if (vtp == #nm) {\
            shd->AddUniform(v.key.string, ShaderVariableType::nm);\
        }
        CE_E_SHV(Float)
        else CE_E_SHV(Color)
        else CE_E_SHV(Texture)
        else CE_E_SHV(CubeMap)
    }
    return shd;
}

CE_END_MOD_AC_NAMESPACE