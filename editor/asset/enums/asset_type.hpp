#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

enum class EAssetType {
    Font,
    Material,
    Mesh,
    Shader,
    Texture,
    _COUNT,
    Unknown
};

CE_END_ED_NAMESPACE