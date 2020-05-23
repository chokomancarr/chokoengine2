#pragma once
#include "chokoeditor.hpp"
#include "ext/visual_shader.hpp"

CE_BEGIN_ED_NAMESPACE

#define CE_E_META_SIGNATURE "ChokoEngine Asset MetaData"
#define CE_E_META_VERSION 20

#define CE_E_DEF(nm) static nm Load ## nm(const std::string& path, bool async)
#define CE_E_AL_IMPL(nm) nm EAssetLoader::Load ## nm(const std::string& path, bool async)

#define CE_E_DEF_EX(nm) static bool Export ## nm(const std::string& path)
#define CE_E_AL_IMPL_EX(nm) bool EAssetLoader::Export ## nm(const std::string& path)

#define CE_E_DEF_J(nm) static nm Load ## nm(const JsonObject& data, SceneObject& obj, bool async)
#define CE_E_AL_IMPL_J(nm) nm EAssetLoader::Load ## nm(const JsonObject& data, SceneObject& obj, bool async)

class EAssetLoader { CE_CLASS_COMMON
    static std::vector<ArmatureBone> LoadBones(const JsonObject& data);

public:

	static void GenDefaultScriptMeta(const std::string& path);
    static void GenDefaultMeta(const std::string& path, const AssetType t);
    static void GenDefaultMeta(const std::string& path, const EExportType t);
	static void GenDefaultMeta(const std::string& path, const EExtType t);

    static Asset Load(const std::string& path, const AssetType t, bool async = false);

    static bool Load(const std::string& path, const EExportType t);
    
    CE_E_DEF_EX(Model);
    CE_E_DEF_EX(Image);

    CE_E_DEF_J(MeshRenderer);
    CE_E_DEF_J(Rig);
};

#undef CE_E_DEF
#undef CE_E_DEF_EX
#undef CE_E_DEF_J

CE_END_ED_NAMESPACE