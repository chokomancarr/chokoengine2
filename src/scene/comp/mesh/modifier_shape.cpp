#include "chokoengine.hpp"
#include "compute/skin_tf_mat.h"

CE_BEGIN_NAMESPACE

TransformFeedback _MeshShapeModifier::_tfProg = 0;

void _MeshShapeModifier::InitResult(size_t n) {
	result = VertexArray_New();
	result->AddBuffer(VertexBuffer_New(true, 3, n, nullptr));
}

void _MeshShapeModifier::InitWeights() {
	const auto& mesh = parent->mesh();
	const auto& vsz = mesh->vertexCount();
    const auto& sks = mesh->shapeKeys();

    const auto sksz = sks.size();

    _weights.resize(sksz, 0);

    std::vector<Vec3> offsets(sksz * vsz); //rearrange for cache friendliness

    for (size_t a = 0; a < sksz; a++) {
        for (size_t b = 0; b < vsz; b++) {
            offsets[b * sksz + a] = sks[a].offsets[b];
        }
    }

	_shpBuf = TextureBuffer::New(
		VertexBuffer_New(true, 3, vsz * sksz, offsets.data()),
		GL_RGB32F
	);
	_whtBuf = TextureBuffer::New(
		VertexBuffer_New(true, 1, sksz, _weights.data()),
		GL_R32F
	);
}

void _MeshShapeModifier::Apply(const VertexArray& vao_in) {
	const auto num = vao_in->buffer(0)->num();
	if (!result || result->buffer(0)->num() != num) {
		InitResult(num);
		result->AddBuffer(vao_in->buffer(1));
		result->AddBuffer(vao_in->buffer(2));
		result->AddBuffer(vao_in->buffer(3));
	}

	auto& mb = _whtBuf->buffer();
	mb->Set(_weights.data(), mb->num());

	_tfProg->vao(vao_in);
	_tfProg->outputs(result->buffers());
	_tfProg->Bind();
	glUniform1i(_tfProg->Loc(1), 1);
	glActiveTexture(GL_TEXTURE1);
	_shpBuf->Bind();
	glUniform1i(_tfProg->Loc(2), 2);
	glActiveTexture(GL_TEXTURE2);
	_whtBuf->Bind();
	_tfProg->Exec();
	_tfProg->Unbind();
}

_MeshShapeModifier::_MeshShapeModifier() : _shpBuf(0), _whtBuf(0) {
	if (!_tfProg) {
		(_tfProg = TransformFeedback_New(compute::skin_tf_mat, { "outPos" }))
			->AddUniforms({ "shps", "whts" });
	}
}

CE_END_NAMESPACE