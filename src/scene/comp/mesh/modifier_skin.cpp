#include "chokoengine.hpp"
#include "compute/skin_tf_mat.h"

CE_BEGIN_NAMESPACE

TransformFeedback _MeshSkinModifier::_tfProg = 0;

void _MeshSkinModifier::InitResult(size_t n) {
	result = VertexArray_New();
	result->AddBuffer(VertexBuffer_New(true, 3, n, nullptr));
	result->AddBuffer(VertexBuffer_New(true, 3, n, nullptr));
	result->AddBuffer(VertexBuffer_New(true, 3, n, nullptr));
}

void _MeshSkinModifier::InitRig() {
	auto p = parent->object()->parent();
	for (auto& o : p->children()) {
		if (!!(_rig = o->GetComponent<Rig>())) {
			InitWeights();
			_matBuf = TextureBuffer::New(VertexBuffer_New(true, 16, _rig->boneObjs().size(), nullptr), GL_RGBA32F);
			return;
		}
	}
	
	Debug::Error("MeshSkinModifier", "Cannot find parent rig to attach to!");
}

void _MeshSkinModifier::InitWeights() {
	const auto& mesh = parent->mesh();
	const auto& vsz = mesh->vertexCount();

	_weightIds.clear();
	_weightIds.resize(vsz, Int4(0));
	_weights.clear();
	_weights.resize(vsz, Vec4(0));

	std::vector<int> bis = {};
	const auto& grps = mesh->vertexGroups();
	for (auto& g : grps) {
		bis.push_back(_rig->BoneIndex(g.name));
	}

	uint noweights = 0;
	for (size_t i = 0; i < vsz; i++) {
		byte a = 0;
		float tot = 0;
		for (size_t g = 0, n = grps.size(); g < n; g++) {
			auto bn = bis[g];
			if (bn == -1) continue;
			const auto w = grps[g].weights[i];
			if (w > 0) {
				_weightIds[i][a] = bn;
				tot += (_weights[i][a] = w);
				if (++a == 4) break;
			}
		}
		if (a == 0) {
			noweights++;
		}
		else {
			while (a > 0) {
				_weights[i][--a] /= tot;
			}
		}
	}

	if (!!noweights) {
		Debug::Warning("SMR", std::to_string(noweights) + " vertices in \"" + mesh->name() + "\" have no weights assigned!");
	}

	_whtIdBuf = TextureBuffer::New(
		VertexBuffer_New(false, 4, vsz, _weightIds.data()),
		GL_RGBA32I
	);
	_whtBuf = TextureBuffer::New(
		VertexBuffer_New(true, 4, vsz, _weights.data()),
		GL_RGBA32F
	);
}

void _MeshSkinModifier::Apply(const VertexArray& vao_in) {
	if (!_rig) InitRig();

	const auto num = vao_in->buffer(0)->num();
	if (!result || result->buffer(0)->num() != num) {
		InitResult(num);
		result->AddBuffer(vao_in->buffer(3));
	}

	auto& mb = _matBuf->buffer();
	mb->Set(_rig->matrices().data(), mb->num());

	_tfProg->vao(vao_in);
	_tfProg->outputs(result->buffers());
	_tfProg->Bind();
	glUniform1i(_tfProg->Loc(1), 1);
	glActiveTexture(GL_TEXTURE1);
	_whtIdBuf->Bind();
	glUniform1i(_tfProg->Loc(2), 2);
	glActiveTexture(GL_TEXTURE2);
	_whtBuf->Bind();
	glUniform1i(_tfProg->Loc(3), 3);
	glActiveTexture(GL_TEXTURE3);
	_matBuf->Bind();
	_tfProg->Exec();
	_tfProg->Unbind();
}

_MeshSkinModifier::_MeshSkinModifier() : _matBuf(0), _whtIdBuf(0), _whtBuf(0) {
	if (!_tfProg) {
		(_tfProg = TransformFeedback_New(compute::skin_tf_mat, { "outPos", "outNrm", "outTgt" }))
			->AddUniforms({ "params", "mids", "mws", "mats" });
	}
}

CE_END_NAMESPACE