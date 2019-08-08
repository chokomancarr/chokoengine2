#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* Applies this mesh's transformations to another mesh.
 * The selected mesh is rendered instead of the original.
 * This modifier has no effect if no other modifiers are
 * applied before it.
 */
class _MeshLatticeModifier : public _MeshModifier { CE_OBJECT_COMMON
	static TransformFeedback _tfProg;

	static void InitProgs();

	Mesh _mesh;

	void InitResult(size_t);
	void InitWeights();

	void Apply(const VertexArray& vao_in) override;

	void OnSetMesh(const Mesh& m) override;

public:
	_MeshLatticeModifier();

	CE_GET_SET_MEMBER_F(mesh);
};

CE_END_NAMESPACE