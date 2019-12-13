#pragma once
#include "chokoplayer_dbg.hpp"

CE_BEGIN_PL_NAMESPACE

class _AssetLoader : public _AssetLoaderBase {
public:
    static void Init();
    
    Asset Load(AssetType, const std::string&) override;
};

CE_END_PL_NAMESPACE