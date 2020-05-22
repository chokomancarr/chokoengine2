#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* Applies a cloth simulation modifier to the mesh.
 * If the mesh is also animated, this modifier should
 * appear after the Skin Modifier.
 */
class _MeshClothModifier : public _MeshModifier { CE_OBJECT_COMMON
	
	static TransformFeedback _tfProg;

    std::vector<float> _pinWeights;

	static void InitProgs();

	void InitResult(size_t);
	void InitWeights();

	bool Apply(const VertexArray& vao_in) override;

	void OnSetMesh(const Mesh& m) override;

public:
	_MeshClothModifier();

    CE_GET_MEMBER(pinWeights);

	friend class _MeshRenderer;
};

CE_END_NAMESPACE