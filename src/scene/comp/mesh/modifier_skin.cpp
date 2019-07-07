#include "chokoengine.hpp"
#include "compute/skin_tf_mat.h"

CE_BEGIN_NAMESPACE

TransformFeedback _MeshSkinModifier::_tfProg = 0;

void _MeshSkinModifier::Apply(const VertexObject& vao_in, const VertexBuffer& elo_in) {
	_tfProg->outputs(vao_in->buffers());
	_tfProg->Bind();

	_tfProg->Exec();
	_tfProg->Unbind();
}

_MeshSkinModifier::_MeshSkinModifier() : _posBuf(0), _nrmBuf(0), _datBuf(0), _matBuf(0), _shpBuf(0), _whtBuf(0) {
	if (!_tfProg) {
		(_tfProg = TransformFeedback_New(compute::skin_tf_mat, { "outPos", "outNrm", "outTgt" }))
			->AddUniforms({
				
			});

	}
}

CE_END_NAMESPACE