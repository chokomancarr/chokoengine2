#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _MeshShapeModifier : public _MeshModifier { CE_OBJECT_COMMON
	TextureBuffer _shpBuf;
	TextureBuffer _whtBuf;

	static TransformFeedback _tfProg;

    std::vector<float> _weights;

	static void InitProgs();

	void InitResult(size_t);
	void InitWeights();

	void Apply(const VertexArray& vao_in) override;

	void OnSetMesh(const Mesh& m) override;

public:
	_MeshShapeModifier();

    CE_GET_MEMBER(weights);
    void SetWeight(const std::string& nm, float v);
    void SetWeight(int i, float v);
};

CE_END_NAMESPACE