#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

CE_ES_IMPL_COMP(MeshRenderer) {
	CE_ES_ADD(mesh);
}

CE_ES_IMPL_COMP_INST(MeshRenderer) {
	auto& c = o->AddComponent<MeshRenderer>();
	CE_ES_SET_A(mesh, Mesh);
}

CE_END_ED_NAMESPACE