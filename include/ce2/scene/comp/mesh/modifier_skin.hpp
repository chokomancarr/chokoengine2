#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _MeshSkinModifier : public _MeshModifier { CE_OBJECT_COMMON
	pRig _rig;
	
	TextureBuffer _matBuf;
	TextureBuffer _whtIdBuf;
	TextureBuffer _whtBuf;

	static TransformFeedback _tfProg;

	std::vector<Int4> _weightIds;
	std::vector<Vec4> _weights;

	static void InitProgs();

	void InitResult(size_t);
	void InitRig();
	void InitWeights();

	void Apply(const VertexArray& vao_in) override;

public:
	_MeshSkinModifier();

	CE_GET_SET_MEMBER(rig);
};

CE_END_NAMESPACE