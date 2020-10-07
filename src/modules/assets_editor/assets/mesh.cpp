#include "asset_loader.hpp"

CE_BEGIN_MOD_AE_NAMESPACE

CE_MOD_AE_IMPL(Mesh) {
	const auto meta = LoadMeta(path);
	const auto ext = StrExt::ExtensionOf(path);
	if (ext == "obj") {
		return MeshLoader::LoadObj(IO::ReadFile(_basePath + path));
	}
	else if (ext == "mesh") {
		return MeshLoader::LoadMesh(_basePath + path);
	}
	else {
		CE_NOT_IMPLEMENTED
	}
}

CE_END_MOD_AE_NAMESPACE
