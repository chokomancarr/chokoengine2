#include "chokoengine.hpp"
#include "glsl/uiVert.h"
#include "glsl/uiTexFrag.h"

BEGIN_CE_NAMESPACE

uint UI::_vboSz;
GLuint UI::_vao, UI::_vboV, UI::_vboU, UI::_tvbo;
GLuint UI::_quadElo;

Shader UI::texShad;

float UI::_alpha;
glm::mat3 UI::matrix;
bool UI::matrixIsI;

bool UI::Init() {
    (texShad = Shader(glsl::uiVert, glsl::uiTexFrag))
        ->AddUniforms({ "tex", "col", "level" });

    _vboSz = 100;
    InitVao();

    _alpha = 1;
    matrix = glm::mat3(1);
    matrixIsI = true;

    return true;
}

void UI::InitVao() {
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vboV);
	glGenBuffers(1, &_vboU);
	glGenTextures(1, &_tvbo);

	glBindVertexArray(_vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _vboV);
	glBufferData(GL_ARRAY_BUFFER, _vboSz * sizeof(Vec3), nullptr, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, _vboU);
	glBufferData(GL_ARRAY_BUFFER, _vboSz * sizeof(Vec2), nullptr, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glGenTextures(1, &_tvbo);
	glBindTexture(GL_TEXTURE_BUFFER, _tvbo);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, _vboV);
	glBindTexture(GL_TEXTURE_BUFFER, 0);

    int d[] = {0, 2, 1, 2, 3, 1};
	glGenBuffers(1, &_quadElo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadElo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), d, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void UI::SetVao(uint sz, void* vts, void* uvs) {
	if (sz > _vboSz) {
    	glDeleteTextures(1, &_tvbo);
		glDeleteBuffers(1, &_vboV);
		glDeleteBuffers(1, &_vboU);
		glDeleteVertexArrays(1, &_vao);
		_vboSz = sz;
		InitVao();
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vboV);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sz * sizeof(Vec3), vts);
	glBindBuffer(GL_ARRAY_BUFFER, _vboU);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sz * sizeof(Vec2), uvs);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

float UI::Dw(float f) {
	return (f / Display::width());
}
float UI::Dh(float f) {
	return (f / Display::height());
}
Vec3 UI::Ds(Vec3 v) {
	return Vec3(Dw(v.x) * 2 - 1, 1 - Dh(v.y) * 2, 1);
}
Vec2 UI::Ds2(Vec2 v) {
	return Vec2(Dw(v.x) * 2 - 1, 1 - Dh(v.y) * 2);
}

void UI::TexQuad(const CE_NS Rect& q, GLuint tex, Color col, 
        int mip, Vec2 uv0, Vec2 uv1, Vec2 uv2, Vec2 uv3) {
    Vec3 quadPoss[4];
	quadPoss[0].x = quadPoss[2].x = q.x();
    quadPoss[0].y = quadPoss[1].y = q.y();
	quadPoss[1].x = quadPoss[3].x = q.x2();
	quadPoss[2].y = quadPoss[3].y = q.y2();
	for (int y = 0; y < 4; ++y) {
		quadPoss[y].z = 1;
		quadPoss[y] = Ds(UI::matrixIsI ? quadPoss[y] : UI::matrix*quadPoss[y]);
	}
	Vec2 quadUvs[4]{ uv0, uv1, uv2, uv3 };

	UI::SetVao(4, quadPoss, quadUvs);

	texShad->Bind();
	glUniform1i(texShad->Loc(0), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform4f(texShad->Loc(1), col.r(), col.g(), col.b(), col.a() * _alpha);
	glUniform1f(texShad->Loc(2), mip);
	glBindVertexArray(_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadElo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void UI::Texture(const CE_NS Rect& rect, const CE_NS Texture& tex, const Color& color) {
    if (!tex || !tex->loaded()) return;
	    TexQuad(rect, tex->_pointer, color);
}

void UI::Rect(const CE_NS Rect& rect, const Color& color) {

}

END_CE_NAMESPACE