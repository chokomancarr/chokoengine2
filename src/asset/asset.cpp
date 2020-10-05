#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Asset::_Asset(AssetType t) : assetType(t), readonly(false), _assetSignature("<instantiated>") {}

CE_END_NAMESPACE