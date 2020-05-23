#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _AnimClip::Entry { CE_CLASS_COMMON
public:
	std::string signature;
	bool isQuat;
	Curve<Vec4> values_v;
	Curve<Quat> values_q;

	VQ Get(float t) const;
};

CE_END_NAMESPACE