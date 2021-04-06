#include "../../getdeps.hpp"

CE_BEGIN_ED_NAMESPACE

CE_DP_BEGIN_IMPL_C(MeshRenderer)

RegMesh(obj->mesh());
for (auto& m : obj->materials()) {
	RegMaterial(m);
}

CE_DP_END_IMPL_C()

CE_END_ED_NAMESPACE