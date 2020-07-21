#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_AnimClip::VQ _AnimClip::VQ::Blend(const VQ& rhs, float t) const {
	if (!valid && !rhs.valid) {
		return _AnimClip::VQ();
	}

	if (!valid) return rhs;
	if (!rhs.valid) return *this;

	VQ res = {};
	res.valid = true;
	res.isQuat = isQuat;
	if (isQuat) {
		res.q = Quat::Slerp(q, rhs.q, t);
	}
	else {
		res.v = Math::Lerp(v, rhs.v, t);
	}
	return res;
}


_AnimClip::_AnimClip() : _Asset(AssetType::AnimClip), _entries({}), _range(0), _repeat(false) {}

float _AnimClip::length() const {
	return _range.y - _range.x + 1;
}

_AnimClip::VQ _AnimClip::Get(const std::string& sig, float t) const {
	VQ res;
	for (auto& e : _entries) {
		if (e.signature == sig) {
			res.valid = true;
			if ((res.isQuat = e.isQuat)) {
				res.q = e.values_q.Eval(t);
			}
			else {
				res.v = e.values_v.Eval(t);
			}
		}
	}
	return res;
}

CE_END_NAMESPACE