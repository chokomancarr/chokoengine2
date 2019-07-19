#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_MeshRenderer::_MeshRenderer() : CE_COMPDEF(MeshRenderer),
		_mesh(nullptr), _modifiers({}), _materials({}) {}

void _MeshRenderer::mesh(const Mesh& m) {
	_mesh = m;
	_materials.clear();
	if (!m) return;
	_materials.resize(m->materialCount(), nullptr);
	for (auto& md : _modifiers) {
		md->OnSetMesh(m);
	}
}

void _MeshRenderer::materials(const std::vector<Material>& m) {
	_materials = m;
	_materials.resize(_mesh->materialCount(), nullptr);
}

void _MeshRenderer::OnUpdate() {
	auto vao = &_mesh->_vao;
	for (auto& m : _modifiers) {
		m->Apply(*vao);
		vao = &m->result;
	}
}

CE_END_NAMESPACE