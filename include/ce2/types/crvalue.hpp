#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/*
 * An object similar to Random::Range
 * The value sampled will lie between
 *   constant ~ constant + random
 */

class CRValue {
public:
	CRValue(float c = 0, float r = 0);

	float constant;
	float random;

	float Sample() const;
};

CE_END_NAMESPACE