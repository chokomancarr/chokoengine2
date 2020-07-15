#include "chokoeditor.hpp"

#define CE_E_LD(nm) case AssetType::nm:\
	Write ## nm((nm)obj);\
	break;

CE_BEGIN_ED_NAMESPACE

void EAssetWriter::Write(const Asset& obj) {
	const auto& sig = obj->assetSignature();
	if (sig == "" || sig == "<instantiated>") {
		return;
	}
	switch (obj->assetType) {
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
}

CE_END_ED_NAMESPACE