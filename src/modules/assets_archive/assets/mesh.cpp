#include "module_aa_inc.hpp"

CE_BEGIN_MOD_AA_NAMESPACE

CE_MOD_AA_IMPL(Mesh) {
	const auto ext = StrExt::ExtensionOf(path);
	if (ext == "obj") {
		return MeshLoader::LoadObj(ArchiveParser::ReadStr(path));
	}
	else if (ext == "mesh") {
		return MeshLoader::LoadMesh(ArchiveParser::GetStrm(path));
	}
	else {
		CE_NOT_IMPLEMENTED
	}
}

CE_END_MOD_AA_NAMESPACE
