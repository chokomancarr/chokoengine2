#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <typename T>
void _MeshRenderer::AddModifier(int index) {
	const auto& mod = T::New();
	mod->parent = pMeshRenderer(shared_from_this());
	_modifiers.push_back(mod);
}

CE_END_NAMESPACE