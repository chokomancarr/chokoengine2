#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_MeshRenderer::_MeshRenderer() : CE_COMPDEF(MeshRenderer),
		_mesh(nullptr), _modifiers({}), _materials({}) {}

void _MeshRenderer::mesh(const Mesh& m) {
	_mesh = m;
	_materials.clear();
	_materials.resize(m->materialCount(), nullptr);
}

void _MeshRenderer::materials(const std::vector<Material>& m) {
	_materials = m;
	_materials.resize(_mesh->materialCount(), nullptr);
}

void _MeshRenderer::AddModifier(const MeshModifier& mod, int index) {

};

CE_END_NAMESPACE