#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

VertexArray EW_S_Rig::_vao;
VertexBuffer EW_S_Rig::_elo;
Shader EW_S_Rig::_boneProg;

void EW_S_Rig::Init() {
	EW_S_DrawCompList::funcs[(int)ComponentType::Rig] = &Draw;

	const float t = 0.2f;
	float poss[] = {
		0, 0, 0,	0, 1, 0,
		t, t, 0,	0, t, t,
		-t, t, 0,	0, t, -t
	};
	int elms[] = {
		0, 2, 3,	0, 4, 5,
		1, 3, 4,	1, 5, 2
	};
	auto pbuf = VertexBuffer_New(true, 3, 6, poss);
	_vao = VertexArray_New();
	_vao->AddBuffer(pbuf);

	_elo = VertexBuffer_New(false, 3, 4, elms);
}

void EW_S_Rig::Draw(const Component& rig) {
	if (rig->object() != ESceneInfo::selectedObject)
		return;
	const auto& arm = (static_cast<Rig>(rig)->armature());
	if (!arm)
		return;

	_vao->Bind();
	_elo->Bind();
	_boneProg->Bind();
	for (auto& b : arm->bones()) {
		
	}
	_boneProg->Unbind();
	_elo->Unbind();
	_vao->Unbind();
}

CE_END_ED_NAMESPACE