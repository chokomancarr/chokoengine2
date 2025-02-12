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
	if (_rig == _attachedRig) return;

	if (parent->object()->parent() != _rig->object()->parent()) {
		Debug::Error("MeshSkinModifier", "rig must be at the same child hierarchy!");
	}
	_attachedRig = _rig;
	InitWeights();
	_matBuf = TextureBuffer::New(VertexBuffer_New(true, 16, _rig->boneObjs().size(), nullptr), GL_RGBA32F);
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
			const auto bn = bis[g];
			if (bn == -1) continue;
			const auto w = grps[g].weights[i];
			if (w > 0) {
				_weightIds[i][a] = bn;
				tot += (_weights[i][a] = w);
				if (++a == 4) break;
			}
		}
		if (!a) {
			noweights++;
		}
		else {
			while (a > 0) {
				_weights[i][--a] /= tot;
			}
		}
	}

	if (!!noweights) {
		Debug::Warning("SMR", std::to_string(noweights) + " out of " + std::to_string(vsz) + " vertices in \"" + mesh->name() + "\" have no weights assigned!");
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

bool _MeshSkinModifier::Apply(const VertexArray& vao_in) {
	if (!_rig) return false;
	InitRig();

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
	const auto& md = parent->object()->transform()->localMatrix();
	const auto imd = md.inverse();
	glUniformMatrix4fv(_tfProg->Loc(0), 1, false, &md[0]);
	glUniformMatrix4fv(_tfProg->Loc(1), 1, false, &imd[0]);
	glUniform1i(_tfProg->Loc(2), 1);
	glActiveTexture(GL_TEXTURE1);
	_whtIdBuf->Bind();
	glUniform1i(_tfProg->Loc(3), 2);
	glActiveTexture(GL_TEXTURE2);
	_whtBuf->Bind();
	glUniform1i(_tfProg->Loc(4), 3);
	glActiveTexture(GL_TEXTURE3);
	_matBuf->Bind();
	_tfProg->Exec();
	_tfProg->Unbind();

	return true;
}

void _MeshSkinModifier::OnSetMesh(const Mesh& m) {}

_MeshSkinModifier::_MeshSkinModifier() : _MeshModifier("Skinning", MeshModifierType::Skin), _matBuf(0), _whtIdBuf(0), _whtBuf(0) {
	if (!_tfProg) {
		(_tfProg = TransformFeedback_New(compute::skin_tf_mat, { "outPos", "outNrm", "outTgt" }))
			->AddUniforms({ "Md", "iMd", "mids", "mws", "mats" });
	}
}

CE_END_NAMESPACE