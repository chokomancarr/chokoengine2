#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_MeshRenderer::_MeshRenderer() : CE_COMPDEF(MeshRenderer),
		_mesh(nullptr), _modifiers({}), _materials({}) {}

_MeshRenderer::_MeshRenderer(const _MeshRenderer& rhs) : CE_COMPDEF(MeshRenderer),
		_mesh(rhs._mesh), _modifiers({}), _materials(rhs._materials) {
	//copy modifiers here
}

void _MeshRenderer::mesh(const Mesh& m) {
	_mesh = m;
	if (!m) {
		_materials.clear();
		return;
	}
	_materials.resize(m->materialCount(), nullptr);
	for (auto& md : _modifiers) {
		md->OnSetMesh(m);
	}
}

void _MeshRenderer::materials(const std::vector<Material>& m) {
	_materials = m;
	_materials.resize(_mesh->materialCount(), nullptr);
}

void _MeshRenderer::OnPostLogic() {
	if (!_mesh) return;
	_vao_final = _mesh->_vao;
	for (auto& m : _modifiers) {
		if (m->enabled) {
			if (m->Apply(_vao_final)) {
				_vao_final = m->result;
			}
		}
	}
}

CE_END_NAMESPACE