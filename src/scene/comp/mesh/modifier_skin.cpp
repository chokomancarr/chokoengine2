#include "chokoengine.hpp"
#include "compute/skin_tf_mat.h"

CE_BEGIN_NAMESPACE

TransformFeedback _MeshSkinModifier::_tfProg = 0;

void _MeshSkinModifier::InitWeights() {
	const auto& vc = _mesh->vertexCount;

	std::vector<uint> noweights;

	weightIds = std::vector<Int4>(_mesh->vertexCount);
	std::vector<SkinDats> dats(_mesh->vertexCount);
	for (uint i = 0; i < _mesh->vertexCount; i++) {
		byte a = 0;
		float tot = 0;
		for (auto& g : _mesh->vertexGroupWeights[i]) {
			auto bn = armature->MapBone(_mesh->vertexGroups[g.first]);
			if (!bn) continue;
			weights[i][a].first = bn;
			weights[i][a].second = g.second;
			if (Features::USE_COMPUTE)
				dats[i].mats_i[a] = bn->id;
			else
				dats[i].mats_v[a] = bn->id;
			tot += g.second;
			if (++a == 4) break;
		}
		for (byte b = a; b < 4; b++) {
			weights[i][b].first = armature->_bones[0];
		}
		if (a == 0) {
			noweights.push_back(i);
			weights[i][0].second = 1;
			dats[i].weights[0] = 1;
		}
		else {
			while (a > 0) {
				weights[i][a - 1].second /= tot;
				dats[i].weights[a - 1] = weights[i][a - 1].second;
				a--;
			}
		}
	}

	if (!!noweights.size())
		Debug::Warning("SMR", std::to_string(noweights.size()) + " vertices in \"" + _mesh->name + "\" have no weights assigned!");
}

void _MeshSkinModifier::Apply(const VertexArray& vao_in) {
	_tfProg->outputs(vao_in->buffers());
	_tfProg->Bind();

	_tfProg->Exec();
	_tfProg->Unbind();
}

_MeshSkinModifier::_MeshSkinModifier() : _posBuf(0), _nrmBuf(0), _datBuf(0), _matBuf(0), _shpBuf(0), _whtBuf(0) {
	if (!_tfProg) {
		(_tfProg = TransformFeedback_New(compute::skin_tf_mat, { "outPos", "outNrm", "outTgt" }))
			->AddUniforms({ "params", "dats", "mats" });

	}
}

CE_END_NAMESPACE