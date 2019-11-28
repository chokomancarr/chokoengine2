#include "chokoeditor.hpp"
#include "utils/glutils.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_S_Camera::Init() {
	EW_S_DrawCompList::funcs[(int)ComponentType::Camera] = &Draw;
	EW_S_DrawCompList::activeFuncs[(int)ComponentType::Camera] = &DrawActive;
}

void EW_S_Camera::Draw(const Component& c, const Mat4x4& p) {
	const auto& cam = static_cast<Camera>(c);
	const auto& tr = cam->object()->transform();

	const auto mvp = p * tr->worldMatrix();

	float vx = 0.35f;
	float vy = 0.25f;
	float vz = 0.5f;

	float verts[] = {
		0, 0, 0,	-vx, -vy, vz,
		vx, -vy, vz,	-vx, vy, vz,
		vx, vy, vz,	0, vy * 1.5f, vz
	};

	int inds[] = {
		0, 1,	0, 2,
		0, 3,	0, 4,
		1, 2,	2, 4,
		4, 3,	3, 1,
		3, 4, 5
	};

	static VertexArray vao = VertexArray_New();
	if (!vao->bufferSize()) {
		vao->AddBuffer(VertexBuffer_New(true, 3, 6, verts));
	}
	static VertexBuffer elo = VertexBuffer_New(false, 1, 19, inds, 0, GL_ELEMENT_ARRAY_BUFFER);

	vao->Bind();
	elo->Bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GLUtils::DrawElemsW(GL_LINES, 16, 0, Color(0), mvp);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLUtils::DrawElemsW(GL_TRIANGLES, 3, 16 * 4, Color(0), mvp);
	elo->Bind();
	vao->Unbind();
}

void EW_S_Camera::DrawActive(const Component& c, const Mat4x4& p) {
	Draw(c, p);

	const auto& cam = static_cast<Camera>(c);
	const auto& tr = cam->object()->transform();

	const auto mvp = p * cam->lastViewProjectionMatrix().inverse();

	float vx = 0.35f;
	float vy = 0.25f;
	float vz = 0.5f;

	float verts[] = {
		-1, -1, -1,		1, -1, -1,
		1, 1, -1,		-1, 1, -1,
		-1, -1, 1,		1, -1, 1,
		1, 1, 1,		-1, 1, 1
	};

	int inds[] = {
		0, 1, 1, 2,		2, 3, 3, 0,
		4, 5, 5, 6,		6, 7, 7, 4,
		0, 4, 1, 5,		2, 6, 3, 7
	};

	static VertexArray vao = VertexArray_New();
	if (!vao->bufferSize()) {
		vao->AddBuffer(VertexBuffer_New(true, 3, 8, verts));
	}
	static VertexBuffer elo = VertexBuffer_New(false, 1, 24, inds, 0, GL_ELEMENT_ARRAY_BUFFER);

	vao->Bind();
	elo->Bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GLUtils::DrawElemsW(GL_LINES, 24, 0, Color(0.5f, 0.5f), mvp);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	elo->Bind();
	vao->Unbind();
}

CE_END_ED_NAMESPACE