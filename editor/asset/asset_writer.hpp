#pragma once
#include "chokoeditor.hpp"

/* For assets with binary data, this updates the metadata only
 * 
 */

#define CE_EAW_DEF(nm) static void Write ## nm(const nm&);
#define CE_EAW_IMPL(nm) void EAssetWriter::Write ## nm(const nm& obj)

CE_BEGIN_ED_NAMESPACE

class EAssetWriter { CE_CLASS_COMMON
public:
	static void Write(const Asset&);

private:

	CE_EAW_DEF(AnimClip);
	CE_EAW_DEF(AnimGraph);
	CE_EAW_DEF(Armature);
	CE_EAW_DEF(Background);
	CE_EAW_DEF(Material);
	CE_EAW_DEF(Mesh);
	CE_EAW_DEF(Prefab);
	CE_EAW_DEF(Shader);
	CE_EAW_DEF(Texture);
};

#undef CE_EAW_DEF

CE_END_ED_NAMESPACE