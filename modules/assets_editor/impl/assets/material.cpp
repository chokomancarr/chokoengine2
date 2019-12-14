#include "asset_loader.hpp"

CE_BEGIN_MOD_AE_NAMESPACE

CE_MOD_AE_IMPL(Material) {
	const auto meta = LoadMeta(path);
	const auto data = JsonParser::Parse(IO::ReadFile(_basePath + path));
	if (data.group[0].key.string != "shader") {
		Debug::Error("AssetLoader::Material", "shader entry missing!");
		return nullptr;
	}
	auto shad = Get<Shader>(AssetType::Shader, data.group[0].value.string);
	auto mat = Material::New();
	mat->shader(shad);
	for (auto& d : data.group) {
		std::string vrnm = "";
		JsonObject vrvl;
		for (auto& d2 : d.value.group) {
			if (d2.key.string == "name")
				vrnm = d2.value.string;
			else if (d2.key.string == "value")
				vrvl = d2.value;
		}
		if (!vrnm.size()) continue;
#define CE_E_ME(tp) if (d.key.string == #tp) {\
			mat->SetUniform(vrnm, vrvl.To ## tp());\
		}
		CE_E_ME(Float)
else CE_E_ME(Color)
		else if (d.key.string == "Texture") {
			mat->SetUniform(vrnm, Get<Texture>(AssetType::Texture, vrvl.string));
		}
	}
	return mat;
}

CE_END_MOD_AE_NAMESPACE
