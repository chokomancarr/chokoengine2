#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template<GLenum T = GL_TEXTURE_2D>
void SetTexParams(int mp = 0, GLenum ws = GL_CLAMP_TO_EDGE, GLenum wt = GL_CLAMP_TO_EDGE, GLenum mn = GL_LINEAR, GLenum mg = GL_LINEAR) {
	if (mp >= 0) glTexParameteri(T, GL_TEXTURE_MAX_LEVEL, mp);
	glTexParameteri(T, GL_TEXTURE_WRAP_S, ws);
	glTexParameteri(T, GL_TEXTURE_WRAP_T, wt);
	glTexParameteri(T, GL_TEXTURE_MIN_FILTER, mn);
	glTexParameteri(T, GL_TEXTURE_MAG_FILTER, mg);
}

template<GLenum T = GL_TEXTURE_2D>
void SetTexParams(const TextureOptions& opts) {
	const GLenum wraps[] = { GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT };
	if (opts.mipmaps >= 0) glTexParameteri(T, GL_TEXTURE_MAX_LEVEL, opts.mipmaps);
	glTexParameteri(T, GL_TEXTURE_WRAP_S, wraps[(int)opts.xwrap]);
	glTexParameteri(T, GL_TEXTURE_WRAP_T, wraps[(int)opts.ywrap]);
	glTexParameteri(T, GL_TEXTURE_MIN_FILTER,
		(opts.linear) ? (
			(opts.mipmaps > 0) ?
			GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR
		) : GL_NEAREST);
	glTexParameteri(T, GL_TEXTURE_MAG_FILTER, (opts.linear) ? GL_LINEAR : GL_NEAREST);
}

class Texture_I {
public:
    static void FlipY(std::vector<byte>& data, uint w, uint h);

    static bool FromJPG(const std::string& path, uint& w, uint& h, byte& channels, std::vector<byte>& data);
	static bool FromJPG(std::istream& strm, size_t n, uint& w, uint& h, byte& channels, std::vector<byte>& data);

    static bool FromPNG(const std::string& path, uint& w, uint& h, byte& channels, std::vector<byte>& data);
	static bool FromPNG(std::istream& strm, size_t n, uint& w, uint& h, byte& channels, std::vector<byte>& data);

    static bool FromBMP(const std::string& path, uint& w, uint& h, byte& channels, std::vector<byte>& data);
	static bool FromBMP(std::istream& strm, size_t n, uint& w, uint& h, byte& channels, std::vector<byte>& data);

	static bool FromHDR(const std::string& path, uint& w, uint& h, byte& channels, std::vector<byte>& data);
	static bool FromHDR(std::istream& strm, size_t n, uint& w, uint& h, byte& channels, std::vector<byte>& data);
};

CE_END_NAMESPACE