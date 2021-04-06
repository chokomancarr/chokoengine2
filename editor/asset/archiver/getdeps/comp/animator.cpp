#include "../../getdeps.hpp"

CE_BEGIN_ED_NAMESPACE

CE_DP_BEGIN_IMPL_C(Animator)

if (obj->single()) {
	RegAnimClip(obj->clip());
}
else {
	RegAnimGraph(obj->graph());
}

CE_DP_END_IMPL_C()

CE_END_ED_NAMESPACE