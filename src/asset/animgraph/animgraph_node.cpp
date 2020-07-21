#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

namespace {
	inline float _len(const AnimClip& clip) {
		return !clip ? 0 : clip->length();
	}
}

_AnimGraph::Node::Node() : type(Type::Single), _clip(nullptr), speed(1), repeat(true), var1D(0) {}

float _AnimGraph::Node::length() const {
	switch (type) {
	case Type::Single:
		return _len(_clip);
	case Type::Linear1D: {
		float v = 0;
		for (auto& c : _clips1D) {
			v = std::max(v, _len(c.second));
		}
		return v;
	}
	}
}

#define _GET(nm) !nm? _AnimClip::VQ() : nm->Get(sig, nm->range().x + t)

_AnimClip::VQ _AnimGraph::Node::Get(const State& st, const std::string& sig, float t) const {
	switch (type) {
	case Type::Single:
		return _GET(_clip);
	case Type::Linear1D:
		const auto blend = st.vars[var1D].f;

		const auto it = std::find_if(_clips1D.begin(), _clips1D.end(), [blend](const std::pair<float, AnimClip>& v) {
			return v.first > blend;
		});
		if (it == _clips1D.begin()) {
			return _GET(it->second);
		}
		else if (it == _clips1D.end()) {
			return _GET((it - 1)->second);
		}
		else {
			const float fac = Math::ILerp((it - 1)->first, it->first, blend);
			return _GET((it - 1)->second).Blend(_GET(it->second), fac);
		}
	}
}

CE_END_NAMESPACE