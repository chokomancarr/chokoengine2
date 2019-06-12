#include "chokoengine.hpp"
#include "glsl/font.h"

BEGIN_CE_NAMESPACE

FT_Library _Font::_ftlib = nullptr;
Shader _Font::_prog;
uint _Font::vaoSz = 0;
GLuint _Font::vao = 0;
GLuint _Font::vbos[] = { 0, 0, 0 };
GLuint _Font::idbuf = 0;

bool _Font::Init() {
	int err = FT_Init_FreeType(&_ftlib);
	if (err != FT_Err_Ok) {
		Debug::Error("_Font", "Initializing Freetype failed!");
		return false;
	}

	(_prog = Shader(glsl::fontVert, glsl::fontFrag))
		->AddUniforms({ "col", "sampler", "mask" });
	return true;
}

void _Font::SizeVec(uint sz) {
	if (vecSize >= sz) return;
	poss.resize(sz * 4 + 1);
	cs.resize(sz * 4);
	uvs.resize(sz * 4);
	ids.resize(sz * 6);
	for (; vecSize < sz; vecSize++) {
		uvs[vecSize * 4] = Vec2(0, 1);
		uvs[vecSize * 4 + 1] = Vec2(1, 1);
		uvs[vecSize * 4 + 2] = Vec2(0, 0);
		uvs[vecSize * 4 + 3] = Vec2(1, 0);
		ids[vecSize * 6] = 4 * vecSize;
		ids[vecSize * 6 + 1] = 4 * vecSize + 1;
		ids[vecSize * 6 + 2] = 4 * vecSize + 2;
		ids[vecSize * 6 + 3] = 4 * vecSize + 1;
		ids[vecSize * 6 + 4] = 4 * vecSize + 3;
		ids[vecSize * 6 + 5] = 4 * vecSize + 2;
	}
}

GLuint _Font::GetGlyph(uint size, uint mask) {
	if (_glyphs.count(size) == 1) {
		auto& gly = _glyphs[size];
		if (gly.count(mask) == 1)
			return _glyphs[size][mask].tex;
	}
	return CreateGlyph(size, mask);
}

GLuint _Font::CreateGlyph(uint sz, uint mask) {
    FT_Set_Pixel_Sizes(_face, 0, sz);
    _glyphs.emplace(sz, std::unordered_map<uint, _glyph_st>(0));
    auto& g = _glyphs[sz][mask];
    glGenTextures(1, &g.tex);
    glBindTexture(GL_TEXTURE_2D, _glyphs[sz][mask].tex);
    std::vector<byte> pixels((sz + 2) * 16 * (sz + 2) * 16, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, (sz + 2) * 16, (sz + 2) * 16, 0, GL_RED, GL_UNSIGNED_BYTE, pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (int a = 0; a < 256; a++) {
        if (FT_Load_Char(_face, mask + a, FT_LOAD_RENDER) != FT_Err_Ok) continue;
        byte x = a % 16, y = a / 16;
        FT_Bitmap bmp = _face->glyph->bitmap;
        glTexSubImage2D(GL_TEXTURE_2D, 0, (sz + 2) * x + 1, (sz + 2) * y + 1, bmp.width, bmp.rows, GL_RED, GL_UNSIGNED_BYTE, bmp.buffer);
        if (bmp.width == 0) {
            g.o2s[a] = 0;
        }
        else {
            g.o2s[a] = _face->glyph->advance.x >> 6;
        }
        g.off[a] = Vec2(_face->glyph->bitmap_left, sz - _face->glyph->bitmap_top);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    if (!mask) {
        g.o2s[(int)' '] = sz * 0.3f;
        g.o2s[(int)'\t'] = sz * 0.9f;
    }
    return g.tex;
}

void _Font::InitVao(uint sz) {
	vaoSz = sz;
	if (!!vao) {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(3, vbos);
		glDeleteBuffers(1, &idbuf); 
	}
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbos);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, vaoSz * sizeof(Vec3), nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, vaoSz * sizeof(Vec2), nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, vaoSz * sizeof(int), nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	glVertexAttribIPointer(2, 1, GL_INT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glGenBuffers(1, &idbuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idbuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sz * 6 * sizeof(uint), nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint _Font::utf2unc(char*& c) {
#define MK(cc) uint(*(cc) & 63)
	if (!((*c >> 7) & 1)) {
		return *(c++);
	}
	else if (!((*c >> 5) & 1)) {
		uint res = *c & 31;
		res = (res << 6) + MK(c+1);
		c += 2;
		return res;
	}
	else if (!((*c >> 4) & 1)) {
		uint res = *c & 15;
		res = (res << 12) + (MK(c + 1) << 6) + MK(c + 2);
		c += 3;
		return res;
	}
	else {
		uint res = *c & 7;
		res = (res << 18) + (MK(c + 1) << 12) + (MK(c + 2) << 6) + MK(c + 3);
		c += 4;
		return res;
	}
}

_Font::_Font(const std::string& path) : _loaded(false), _alignment(FontAlign::TopLeft), vecSize(0) {
	auto err = FT_New_Face(_ftlib, path.c_str(), 0, &_face);
	if (err != FT_Err_Ok) {
		Debug::Warning("_Font", "Freetype loader failed with error code " + std::to_string(err));
		return;
	}
	FT_Select_Charmap(_face, FT_ENCODING_UNICODE);
	CreateGlyph(12, 0);
	SizeVec(20);
	_loaded = true;
}

END_CE_NAMESPACE