#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

#define CE_E_META_SIGNATURE "ChokoEngine Asset MetaData"
#define CE_E_META_VERSION 20

#define CE_E_DEF(nm) static nm Load ## nm(const std::string& path)

class EAssetLoader { CE_CLASS_COMMON
    static JsonObject LoadMeta(const std::string& path);

public:
    static Asset Load(const std::string& path, const EAssetType t);

    CE_E_DEF(Texture);
};

#undef CE_E_DEF

CE_END_ED_NAMESPACE