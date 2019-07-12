#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _AnimClip::Entry { CE_OBJECT_COMMON
public:
	std::string signature;
	Curve<Value> values;
};

CE_END_NAMESPACE