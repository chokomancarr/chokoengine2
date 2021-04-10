#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <typename T>
T _MeshRenderer::AddModifier(int index) {
	static_assert(std::is_base_of<_MeshModifier, typename T::_TpBase>::value, "T must be a mesh modifier type!");
	auto mod = T::New();
	mod->parent = get_shared<_MeshRenderer>();
	_modifiers.push_back(mod);
	if (!!_mesh) {
		mod->OnSetMesh(_mesh);
	}
	return mod;
}

CE_END_NAMESPACE