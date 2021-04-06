#include "../../getdeps.hpp"

CE_BEGIN_ED_NAMESPACE

CE_DP_BEGIN_IMPL_A(AnimGraph)

for (auto& st : obj->nodes()) {
	switch (st.type) {
	case _AnimGraph::Node::Type::Single:
		RegAnimClip(st.clip());
		break;
	case _AnimGraph::Node::Type::Linear1D:
		for (auto& c : st.clips1D()) {
			RegAnimClip(c.second);
		}
		break;
	}
}

CE_DP_END_IMPL_A()

CE_END_ED_NAMESPACE