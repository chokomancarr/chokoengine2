#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_AnimClip::_AnimClip() : _Asset(AssetType::AnimClip), _entries({}), _range(0), _repeat(false) {}

CE_END_NAMESPACE