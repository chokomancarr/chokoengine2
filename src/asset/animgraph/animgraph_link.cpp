#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

namespace {
	typedef _AnimGraph::Link::Cond::Comp _C;
	std::vector<_AnimGraph::Link::Cond::Comp> getmatches(const _AnimGraph::_Var& val, const _AnimGraph::_Var& def, const _AnimGraph::Var::Type tp) {
		switch (tp) {
		case _AnimGraph::Var::Type::Bool:
			if (val.b == def.b) {
				return { _C::Equals };
			}
			else {
				return { _C::NEquals };
			}
			break;
		case _AnimGraph::Var::Type::Int:
			if (val.i == def.i) {
				return { _C::Equals, _C::GEquals, _C::LEquals };
			}
			else if (val.i > def.i) {
				return { _C::Greater, _C::GEquals, _C::NEquals };
			}
			else {
				return { _C::Less, _C::LEquals, _C::NEquals };
			}
			break;
		case _AnimGraph::Var::Type::Float:
			if (val.f == def.f) {
				return { _C::Equals, _C::GEquals, _C::LEquals };
			}
			else if (val.f > def.f) {
				return { _C::Greater, _C::GEquals, _C::NEquals };
			}
			else {
				return { _C::Less, _C::LEquals, _C::NEquals };
			}
			break;
		}
	}
}

bool _AnimGraph::Link::Cond::Check(const _AnimGraph& g, const State& st) const {
	const auto mts = getmatches(st.vars[paramId], value, g._vars[paramId].type);
	return std::find(mts.begin(), mts.end(), comp) != mts.end();
}

CE_END_NAMESPACE