#include "../../getdeps.hpp"

CE_BEGIN_ED_NAMESPACE

CE_DP_BEGIN_IMPL_A(Material)

RegShader(obj->shader());

for (auto& v : obj->variables()) {
	switch (v.type()) {
	case ShaderVariableType::Texture:
		RegTexture(v.val_t());
		break;
	default:
		break;
	}
}

CE_DP_END_IMPL_A()

CE_END_ED_NAMESPACE