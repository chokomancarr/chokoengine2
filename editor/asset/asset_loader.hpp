#pragma once
#include "chokoeditor.hpp"
#include "parsers/json.hpp"

CE_BEGIN_ED_NAMESPACE

#define CE_E_META_SIGNATURE "ChokoEngine Asset MetaData"
#define CE_E_META_VERSION 20

#define CE_E_DEF(nm) static nm Load ## nm(const std::string& path)
#define CE_E_AL_IMPL(nm) nm EAssetLoader::Load ## nm(const std::string& path)

#define CE_E_DEF_J(nm) static nm Load ## nm(const JsonObject& data, SceneObject& obj)
#define CE_E_AL_IMPL_J(nm) nm EAssetLoader::Load ## nm(const JsonObject& data, SceneObject& obj)

class EAssetLoader { CE_CLASS_COMMON
    static JsonObject LoadMeta(const std::string& path);

    static SceneObject JsonToObject(const JsonObject& data);

    static std::vector<Bone> LoadBones(const JsonObject& data);
public:
    static void GenDefaultMeta(const std::string& path, const EAssetType t);

    static Object Load(const std::string& path, const EAssetType t);

    CE_E_DEF(Armature);
    CE_E_DEF(Material);
    CE_E_DEF(Mesh);
    CE_E_DEF(Shader);
    CE_E_DEF(Texture);
    CE_E_DEF(SceneObject);

    CE_E_DEF_J(MeshRenderer);
};

#undef CE_E_DEF

CE_END_ED_NAMESPACE