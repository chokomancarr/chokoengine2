#include "chokoeditor.hpp"
#include "ce2/modules/ae/asset_loader.hpp"
#include "exporters/blender.hpp"
#include "templates/meta/meta_common.hpp"

CE_BEGIN_ED_NAMESPACE

std::vector<ArmatureBone> EAssetLoader::LoadBones(const JsonObject& data) {
	std::vector<ArmatureBone> res;
	res.reserve(data.group.size());
	for (auto& g : data.group) {
		ArmatureBone b(g.key.string);
		for (auto& p : g.value.group) {
			if (p.key.string == "head") {
				b.base = p.value.ToVec3();
			}
			else if (p.key.string == "tail") {
				b.length = p.value.ToVec3();
			}
			else if (p.key.string == "front") {
				b.front = p.value.ToVec3();
			}
			else if (p.key.string == "connected") {
				b.connected = p.value.ToBool();
			}
			else if (p.key.string == "children") {
				b.children = LoadBones(p.value);
			}
		}
		b.length -= b.base;
		res.push_back(b);
	}
	return res;
}

#define CE_E_MKM(t, nm) case t::nm: {\
	std::ofstream strm(CE_DIR_ASSET + path + ".meta");\
	strm << meta::nm;\
	break;\
}

void EAssetLoader::GenDefaultScriptMeta(const std::string& path) {
	std::ofstream strm(CE_DIR_ASSET + path + ".meta");
	strm << meta::Script;
}

void EAssetLoader::GenDefaultMeta(const std::string& path, const AssetType t) {
	switch (t) {
		CE_E_MKM(AssetType, AnimClip)
		CE_E_MKM(AssetType, Armature)
		CE_E_MKM(AssetType, Background)
		CE_E_MKM(AssetType, Material)
		CE_E_MKM(AssetType, Mesh)
		CE_E_MKM(AssetType, Prefab)
		CE_E_MKM(AssetType, Shader)
		CE_E_MKM(AssetType, Texture)
		default:
			break;
	}
}

void EAssetLoader::GenDefaultMeta(const std::string& path, const EExportType t) {
	switch (t) {
		CE_E_MKM(EExportType, Model)
		CE_E_MKM(EExportType, Image)
		default:
			break;
	}
}

void EAssetLoader::GenDefaultMeta(const std::string& path, const EExtType t) {
	switch (t) {
	default:
		break;
	}
}

#define CE_E_LD(nm) case AssetType::nm: {\
	auto res = Load ## nm(path, async);\
	if (!res)\
		Debug::Error("EAssetLoader", "Failed to load \"" + path + "\" (nullptr returned by loader)!");\
	res->assetSignature(path);\
	res->name(StrExt::RemoveExt(StrExt::RemoveFd(path)));\
	return res; }

Asset EAssetLoader::Load(const std::string& path, const AssetType t, bool async) {
	return ModuleAE::AssetLoader::Get<Asset>(t, path, async, false);
}

#undef CE_E_LD
#define CE_E_EX(nm) case EExportType::nm:\
	result = Export ## nm(path);\
	break;

bool EAssetLoader::Load(const std::string& path, const EExportType t) {
	bool result;
	switch (t) {
		CE_E_EX(Model)
		CE_E_EX(Image)
		default:
			return false;
	}
	if (!result) {
		Debug::Warning("Asset Loader", "Exporter operation failed!");
	}
	return result;
}

#undef CE_E_EX

CE_E_AL_IMPL_EX(Model) {
	const auto meta = ModuleAE::AssetLoader::LoadMeta(path);
	const auto ext = StrExt::ExtensionOf(path);
	if (ext == "blend") {
		return BlenderExporter::ExportBlend(CE_DIR_ASSET + path, CE_DIR_ASSET, ".exported/" + path + "/");
	}
	else CE_ABORT(); //we should never get here
}

CE_E_AL_IMPL_EX(Image) {
	const auto meta = ModuleAE::AssetLoader::LoadMeta(path);
	const auto ext = StrExt::ExtensionOf(path);
	if (ext == "psd") {
		return BlenderExporter::ExportImage(CE_DIR_ASSET + path, CE_DIR_ASSET + ".exported/" + path + "/");
	}
	else CE_ABORT(); //we should never get here
}

CE_END_ED_NAMESPACE