#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _MeshSkinModifier : _MeshModifier { CE_OBJECT_COMMON
	TextureBuffer _posBuf;
	TextureBuffer _nrmBuf;
	TextureBuffer _datBuf;
	TextureBuffer _matBuf;
	TextureBuffer _shpBuf;
	TextureBuffer _whtBuf;

	static TransformFeedback _tfProg;

	static void InitProgs();

	void Apply(const VertexArray& vao_in, const VertexBuffer& elo_in) override;

public:
	_MeshSkinModifier();
};

CE_END_NAMESPACE