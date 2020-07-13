#include "chokoeditor.hpp"
#include "glsl/arm_bone.h"
#include "ext/glmext.hpp"

CE_BEGIN_ED_NAMESPACE

VertexArray EW_S_Rig::_vao;
VertexBuffer EW_S_Rig::_elo;
Shader EW_S_Rig::_boneProg;

void EW_S_Rig::Init() {
	EW_S_DrawCompList::activeFuncs[(int)ComponentType::Rig] = &DrawActive;

	const float t = 0.2f;
	float poss[] = {
		0, 0, 0,	0, 0, 1,
		t, t, t,	t, -t, t,
		-t, -t, t,	-t, t, t
	};
	int elms[] = {
		0, 2, 3,	0, 4, 5,
		1, 3, 4,	1, 5, 2
	};
	auto pbuf = VertexBuffer_New(true, 3, 6, poss);
	_vao = VertexArray_New();
	_vao->AddBuffer(pbuf);

	_elo = VertexBuffer_New(false, 1, 12, elms, 0, GL_ELEMENT_ARRAY_BUFFER);

	(_boneProg = Shader::New(glsl::armBoneVert, glsl::armBoneFrag))
		->AddUniforms({ "MVP", "len", "color" });
}

void EW_S_Rig::DrawActive(const Component& c, const Mat4x4& p) {
	const auto& rig = static_cast<Rig>(c);
	const auto& arm = (rig->armature());
	if (!arm)
		return;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	_vao->Bind();
	_elo->Bind();
	_boneProg->Bind();
	for (auto& b : rig->boneObjs()) {
		const auto& MV = b.obj->transform()->worldMatrix();
		glUniformMatrix4fv(_boneProg->Loc(0), 1, false, fptr(p * MV));
		glUniform1f(_boneProg->Loc(1), b.bone.length);
		if (b.obj.lock() == ESceneInfo::selectedObject) {
			glUniform4f(_boneProg->Loc(2), 1, 1, 0, 1);
		}
		else {
			glUniform4f(_boneProg->Loc(2), 1, 1, 1, 1);
		}
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	}
	_boneProg->Unbind();
	_elo->Unbind();
	_vao->Unbind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

CE_END_ED_NAMESPACE