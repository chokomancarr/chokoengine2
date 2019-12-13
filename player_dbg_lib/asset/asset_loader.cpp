#pragma once
#include "chokoplayer_dbg.hpp"

CE_BEGIN_PL_NAMESPACE

void _AssetLoader::Init() {
    instance = std::make_shared<_AssetLoader>();
}

Asset _AssetLoader::Load(AssetType, const std::string&) {

}

CE_END_PL_NAMESPACE