#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

bool _Light::Init() {
    return true;
}

_Light::_Light(LightType t) : CE_COMPDEF(Light), _type(t), _falloff(LightFalloff::InverseSquared),
        _strength(1), _distance(10), _radius(0), _shadowTarget(nullptr), _shadow(false),
        _shadowDistance(50), _shadowStrength(1), _shadowOnly(false) {}

CE_END_NAMESPACE