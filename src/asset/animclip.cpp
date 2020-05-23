#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_AnimClip::_AnimClip() : _Asset(AssetType::AnimClip), _entries({}), _range(0), _repeat(false) {}

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