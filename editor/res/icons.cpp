#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::unordered_map<std::string, Texture> EIcons::icons = {};

void EIcons::Init() {
	const auto base_path = IO::path() + "res/icons/";
	auto list = JsonParser::Parse(IO::ReadFile(base_path + "icons_manifest.json"));
	for (auto& i : list.group) {
		icons[i.key.string] = Texture::New(base_path + i.value.string);
	}
}

CE_END_ED_NAMESPACE