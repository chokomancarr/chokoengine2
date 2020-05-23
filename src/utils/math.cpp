#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <>
Quat Math::Lerp<Quat, float>(const Quat& a, const Quat& b, const float& t) {
	return Quat::Slerp(a, b, t);
}

CE_END_NAMESPACE