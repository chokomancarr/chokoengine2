#pragma once
#include "chokoeditor.hpp"
#include "ce2/parsers/json.hpp"

CE_BEGIN_ED_NAMESPACE

#define CE_E_META_SIGNATURE "ChokoEngine Asset MetaData"
#define CE_E_META_VERSION 20

#define CE_E_DEF(nm) static nm Load ## nm(const std::string& path)
#define CE_E_AL_IMPL(nm) nm EAssetLoader::Load ## nm(const std::string& path)

#define CE_E_DEF_EX(nm) static void Export ## nm(const std::string& path)
#define CE_E_AL_IMPL_EX(nm) void EAssetLoader::Export ## nm(const std::string& path)

#define CE_E_DEF_J(nm) static nm Load ## nm(const JsonObject& data, SceneObject& obj)
#define CE_E_AL_IMPL_J(nm) nm EAssetLoader::Load ## nm(const JsonObject& data, SceneObject& obj)

class EAssetLoader { CE_CLASS_COMMON
    static JsonObject LoadMeta(const std::string& path);

    static SceneObject JsonToObject(const JsonObject& data);

    static std::vector<Bone> LoadBones(const JsonObject& data);
public:
    static void GenDefaultMeta(const std::string& path, const EAssetType t);

    static void GenDefaultMeta(const std::string& path, const EExportType t);

    static Asset Load(const std::string& path, const EAssetType t);

    static void Load(const std::string& path, const EExportType t);

	CE_E_DEF(AnimClip);
    CE_E_DEF(Armature);
    CE_E_DEF(Material);
    CE_E_DEF(Mesh);
    CE_E_DEF(Shader);
    CE_E_DEF(Texture);
    CE_E_DEF(SceneObject);

    CE_E_DEF_EX(Model);
    CE_E_DEF_EX(Image);

    CE_E_DEF_J(MeshRenderer);
    CE_E_DEF_J(Rig);
};

#undef CE_E_DEF
#undef CE_E_DEF_EX
#undef CE_E_DEF_J

CE_END_ED_NAMESPACE