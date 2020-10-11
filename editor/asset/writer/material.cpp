#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

CE_EAW_IMPL(Material) {
	JsonObject res(JsonObject::Type::Group);
	res.group.push_back(JsonPair(JsonObject("shader"), 
		!obj->shader() ? "None" : obj->shader()->assetSignature()));

#define CS(nm)\
	case ShaderVariableType::nm:\
		tnm = #nm;

	std::string tnm;
	JsonObject vvl;
	for (auto& v : obj->variables()) {
		switch (v.type()) {
			CS(Float) {
				vvl = std::to_string(v.val_f());
				break;
			}
			CS(Color) {
				vvl = JsonObject::FromVec4(v.val_v4());
			}
			CS(Texture) {
				vvl = !v.val_t() ? "None" : v.val_t()->assetSignature();
				break;
			}
		default:
			EDebug::Log("AssetWriter::Material", "ignored variable " + v.name() + " of type "
				+ std::to_string((int)v.type()) + "!", EDEBUG_LVL_WARNING);
			continue;
		}
		res.group.push_back(JsonPair(tnm, JsonObject({
			JsonPair(JsonObject("name"), v.name()),
			JsonPair(JsonObject("value"), vvl)
		})));
	}

	std::ofstream(CE_DIR_ASSET + obj->assetSignature()) << JsonParser::Export(res);
}

CE_END_ED_NAMESPACE
