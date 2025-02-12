#include "asset_loader.hpp"

CE_BEGIN_MOD_AE_NAMESPACE

namespace {
	class _AssetLoaderImpl : public CE_PL_NS _AssetLoaderBase {
	public:
		static void Init() {
			instance = std::make_shared<_AssetLoaderImpl>();
		}

		Asset Load(AssetType t, const std::string& s) override {
			return AssetLoader::Load(t, s, true);
		}
	};
}

std::string AssetLoader::_basePath = "";
std::array<std::unordered_map<std::string, Asset>, (int)AssetType::_COUNT> AssetLoader::_cache;

void AssetLoader::Init(const std::string& basePath) {
	_basePath = basePath;
	_AssetLoaderImpl::Init();
}

JsonObject AssetLoader::LoadMeta(const std::string& path) {
	auto obj = JsonParser::Parse(IO::ReadFile(_basePath + path + ".meta"));
	if (obj.type != JsonObject::Type::Group) {
		Debug::Warning("Asset MetaData", "Invalid file contents!");
		return {};
	}
	if (obj.group.size() < 2) {
		Debug::Warning("Asset MetaData", "Invalid file contents!");
		return {};
	}
	if (obj.group[0].key.string != "signature") {
		Debug::Warning("Asset MetaData", "Signature entry missing!");
		return {};
	}
	if (obj.group[0].value.string != CE_MOD_AE_META_SIGNATURE) {
		Debug::Warning("Asset MetaData", "Signature is incorrect!");
		return {};
	}
	if (obj.group[1].key.string != "version") {
		Debug::Warning("Asset MetaData", "Version entry missing!");
		return {};
	}
	//check for version compat here
	return obj;
}

#define CE_E_LD(nm) case AssetType::nm: {\
	auto res = Load ## nm(path, async);\
	if (!res)\
		Debug::Error("AssetLoader Module", "Failed to load \"" + path + "\" (nullptr returned by loader)!");\
	res->assetSignature(path);\
	res->name(StrExt::RemoveExt(StrExt::RemoveFd(path)));\
	return res; }

Asset AssetLoader::Load(AssetType t, const std::string& path, bool async) {
	if (path == "none") return nullptr;
	switch (t) {
		CE_E_LD(AnimClip)
		CE_E_LD(AnimGraph)
		CE_E_LD(Armature)
		CE_E_LD(Background)
		CE_E_LD(Material)
		CE_E_LD(Mesh)
		CE_E_LD(Prefab)
		CE_E_LD(Shader)
		CE_E_LD(Texture)
	default:
		CE_NOT_IMPLEMENTED
	}
	return nullptr;
}

CE_END_MOD_AE_NAMESPACE