#pragma once
#include "chokoengine.hpp"
#include "enums/font_align.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

BEGIN_CE_NAMESPACE

class _Font { CE_OBJECT_COMMON

    static FT_Library _ftlib;
    static Shader _prog;
	static uint vaoSz;
	static GLuint vao, vbos[3], idbuf;
    
    bool _loaded;
    
	FT_Face _face;
	FontAlign _alignment;

	struct _glyph_st {
        GLuint tex;
		float o2s[256];
		Vec2 off[256];
	};
	std::unordered_map<uint, std::unordered_map<uint, _glyph_st>> _glyphs;

	uint vecSize;
	std::vector<Vec3> poss;
	std::vector<Vec2> uvs;
	std::vector<uint> ids;
	std::vector<uint> cs;

	static bool Init();

    void InitVao(uint sz);
	void SizeVec(uint sz);
    
	GLuint GetGlyph(uint size, uint mask);
	GLuint CreateGlyph(uint size, uint mask = 0);
    
	static uint utf2unc(char*& c);
public:

	GET_MEMBER_FUNC(bool, loaded);
	GET_SET_MEMBER_FUNC(FontAlign, alignment);

    /* Constructs a font from a .ttf file
     */
	_Font(const std::string& path);
};

END_CE_NAMESPACE