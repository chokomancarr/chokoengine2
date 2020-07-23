#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

CRValue::CRValue(float c, float r) : constant(c), random(r) {}

float CRValue::Sample() const {
	return constant + Random::Value() * random;
}

CE_END_NAMESPACE