#include "chokoengine.hpp"
#include "glsl/uiColVert.h"
#include "glsl/uiColFrag.h"
#include "glsl/uiTexVert.h"
#include "glsl/uiTexFrag.h"

CE_BEGIN_NAMESPACE

uint UI::_vboSz;
GLuint UI::_vao, UI::_vboV, UI::_vboU, UI::_tvbo;
GLuint UI::_quadElo;

Shader UI::colShad;
Shader UI::texShad;

float UI::_alpha;
glm::mat3 UI::matrix;
bool UI::matrixIsI;

bool UI::Init() {
	(colShad = Shader::New(glsl::uiColVert, glsl::uiColFrag))
		->AddUniforms({ "col" });

    (texShad = Shader::New(glsl::uiTexVert, glsl::uiTexFrag))
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
	if (uvs) {
		glBindBuffer(GL_ARRAY_BUFFER, _vboU);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sz * sizeof(Vec2), uvs);
	}
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

std::array<Vec3, 4> UI::GetQuadVecs(const CE_NS Rect& q) {
	std::array<Vec3, 4> quadPoss;
	quadPoss[0].x = quadPoss[2].x = q.x();
	quadPoss[0].y = quadPoss[1].y = q.y();
	quadPoss[1].x = quadPoss[3].x = q.x2();
	quadPoss[2].y = quadPoss[3].y = q.y2();
	for (int y = 0; y < 4; ++y) {
		quadPoss[y].z = 1;
		quadPoss[y] = Ds(UI::matrixIsI ? quadPoss[y] : UI::matrix*quadPoss[y]);
	}
	return quadPoss;
}

void UI::TexQuad(const CE_NS Rect& q, GLuint tex, Color col, 
        int mip, Vec2 uv0, Vec2 uv1, Vec2 uv2, Vec2 uv3) {
	auto quadPoss = GetQuadVecs(q);
	Vec2 quadUvs[4]{ uv0, uv1, uv2, uv3 };

	UI::SetVao(4, quadPoss.data(), quadUvs);

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

void UI::Rect(const CE_NS Rect& q, const Color& col) {
	auto quadPoss = GetQuadVecs(q);
	UI::SetVao(4, quadPoss.data(), nullptr);

	colShad->Bind();
	glUniform4f(colShad->Loc(0), col.r(), col.g(), col.b(), col.a() * _alpha);
	glBindVertexArray(_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadElo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void UI::Label(const CE_NS Rect& rect, const std::string& str, const Color& col, const Font& font) {
	const auto fsz = font->size();
	const auto ssz = str.size();
	if (!fsz || !ssz) return;
	
	byte align = (byte)font->alignment();

	//get the unicode string
	auto ucs = StrExt::ToUnicode(str);
	const auto usz = ucs.size();
	std::vector<uint> mks;

	//preload glyphs
	for (auto c : ucs) {
		c &= 0xff00;
		if (std::find(mks.begin(), mks.end(), c) == mks.end()) {
			mks.push_back(c);
			font->GetGlyph(fsz, c);
		}
	}

	//get the total width
	auto& params = font->_glyphs[fsz];
	float strw = 0;
	for (uint i = 0; i < usz; ++i) {
		const auto& c = ucs[i];
		strw += params[c & 0xff00].o2s[c & 0x00ff];
	}

	//get the starting x
	float xpos = rect.x();
	switch (align & 0x0f) {
		case 0: break;
		case 1: {
			xpos = rect.x() + (rect.w() - strw) / 2;
		}
		case 2: {
			xpos = rect.x() + rect.w() - strw;
		}
		default: break;
	}
	xpos = std::roundf(xpos);

	//get the starting y
	float ypos = rect.y();
	switch (align >> 4) {
		case 0: break;
		case 1: {
			ypos = rect.y() + (rect.h() - fsz) / 2;
		}
		case 2: {
			ypos = rect.y() + rect.h() - fsz;
		}
		default: break;
	}
	ypos = std::roundf(ypos);

	//set quads
	for (size_t i = 0; i < usz * 4; i += 4) {
		auto& c = ucs[i / 4];
		auto m = c & 0xff00;
		auto cc = c & 0x00ff;
		auto& p = params[m];

		Vec3 off = Vec3(p.off[cc].x, -p.off[cc].y, 0);

		font->poss[i] = Ds(Vec3(xpos - 2, ypos + fsz + 2, 1) + off);
		font->poss[i + 1] = Ds(Vec3(xpos + fsz + 2, ypos + fsz + 2, 1) + off);
		font->poss[i + 2] = Ds(Vec3(xpos - 2, ypos + 2, 1) + off);
		font->poss[i + 3] = Ds(Vec3(xpos + fsz + 2, ypos + 2, 1) + off);
		std::memset(&font->cs[i], c, 4 * sizeof(uint));
		xpos += p.o2s[cc];
		if (c == (uint)' ' || c == (uint)'\t')
			xpos = std::roundf(xpos);
	}
	font->poss[usz * 4] = Ds(Vec3(xpos, 0, 0));

	_Font::vao->buffer(0)->Set(font->poss.data(), usz * 4);
	_Font::vao->buffer(1)->Set(font->cs.data(), usz * 4);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	_Font::vao->Bind();
	_Font::idbuf->Bind();
	_Font::_prog->Bind();
	glUniform4f(_Font::_prog->Loc(0), col.r(), col.g(), col.b(), col.a() * _alpha);
	glUniform2f(_Font::_prog->Loc(2), 0, 0);
	for (auto& m : mks) {
		const GLuint tex = font->GetGlyph(fsz, 0);
		glUniform1i(_Font::_prog->Loc(1), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(_Font::_prog->Loc(3), m);

		glDrawElements(GL_TRIANGLES, 6 * usz, GL_UNSIGNED_INT, 0);
	}
	_Font::_prog->Unbind();
	_Font::idbuf->Unbind();
	_Font::vao->Unbind();
}

CE_END_NAMESPACE