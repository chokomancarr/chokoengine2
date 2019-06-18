#include "chokoengine.hpp"
#include "glsl/uiColVert.h"
#include "glsl/uiColFrag.h"
#include "glsl/uiTexVert.h"
#include "glsl/uiTexFrag.h"

CE_BEGIN_NAMESPACE

uint UI::_vboSz;
VertexBuffer UI::_vboV, UI::_vboU;
VertexObject UI::_vao;
TextureBuffer UI::_tvbo;
VertexBuffer UI::_quadElo;

Shader UI::colShad;
Shader UI::texShad;

float UI::_alpha;
glm::mat3 UI::matrix;
bool UI::matrixIsI;

Font UI::_defaultFont;

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
	int d[] = { 0, 2, 1, 2, 3, 1 };

	_vboV = VertexBuffer_New(true, 3, _vboSz, nullptr);
	_vboU = VertexBuffer_New(true, 2, _vboSz, nullptr);
	_vao = VertexObject_New();
	_vao->AddBuffer(_vboV);
	_vao->AddBuffer(_vboU);
	_tvbo = TextureBuffer_New(_vboV, GL_RGB32F);
	_quadElo = VertexBuffer_New(false, 1, 6, d, 0, GL_ELEMENT_ARRAY_BUFFER);
}

void UI::SetVao(uint sz, void* vts, void* uvs) {
	if (sz > _vboSz) {
		_vboSz = sz;
		InitVao();
	}
	_vboV->Set(vts, sz);
	if (uvs) {
		_vboU->Set(uvs, sz);
	}
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
	_vao->Bind();
	_quadElo->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	_quadElo->Unbind();
	_vao->Unbind();
	texShad->Unbind();
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
	_vao->Bind();
	_quadElo->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	_quadElo->Unbind();
	_vao->Unbind();
	colShad->Unbind();
}

void UI::Label(const CE_NS Rect& rect, const std::string& str, const Color& col, Font font) {
	if (!font || !font->loaded()) {
		if (!_defaultFont || !_defaultFont->loaded()) {
			Debug::Warning("UI::Label", "Font provided is null!");
			return;
		}
		font = _defaultFont;
	}
	const auto fsz = font->size();
	const auto ssz = str.size();
	if (!fsz || !ssz) return;
	
	byte align = (byte)font->alignment();

	//get the unicode string
	auto ucs = StrExt::ToUnicode(str);
	const auto usz = ucs.size();
	std::vector<uint> mks = {};

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

		Vec3 off = Vec3(p.off[cc].x, p.off[cc].y, 0);

		const auto xpos0 = xpos - 1;
		const auto xpos1 = xpos + fsz + 1;
		const auto ypos0 = ypos - 1;
		const auto ypos1 = ypos + fsz + 1;
		font->poss[i] = Ds(Vec3(xpos0, ypos1, 1) + off);
		font->poss[i + 1] = Ds(Vec3(xpos1, ypos1, 1) + off);
		font->poss[i + 2] = Ds(Vec3(xpos0, ypos0, 1) + off);
		font->poss[i + 3] = Ds(Vec3(xpos1, ypos0, 1) + off);
		font->cs[i] = font->cs[i + 1] = font->cs[i + 2] = font->cs[i + 3] = c;
		xpos += p.o2s[cc];
		if (c == (uint)' ' || c == (uint)'\t')
			xpos = std::roundf(xpos);
	}
	font->poss[usz * 4] = Ds(Vec3(xpos, 0, 0));

	_Font::vao->buffer(0)->Set(font->poss.data(), usz * 4);
	_Font::vao->buffer(1)->Set(font->cs.data(), usz * 4);

	_Font::_prog->Bind();
	_Font::vao->Bind();
	_Font::idbuf->Bind();
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
	_Font::idbuf->Unbind();
	_Font::vao->Unbind();
	_Font::_prog->Unbind();

	UI::TexQuad(CE_NS Rect(300, 300, 14 * 16, -14 * 16), font->GetGlyph(12, 0));
}

CE_END_NAMESPACE