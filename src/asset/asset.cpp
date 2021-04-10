#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Asset::_Asset(AssetType t) : _assetSignature("<instantiated>"), assetType(t), readonly(false) {}

CE_END_NAMESPACE