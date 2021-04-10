#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <>
Quat Math::Lerp<Quat, float>(const Quat& a, const Quat& b, const float& t, bool clamped) {
	if (clamped) {
		if (t <= 0) return a;
		else if (t >= 1) return b;
	}
	return Quat::Slerp(a, b, t);
}

CE_END_NAMESPACE