#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template<GLenum T = GL_TEXTURE_2D>
void SetTexParams(int mp = 0, GLenum ws = GL_CLAMP_TO_EDGE, GLenum wt = GL_CLAMP_TO_EDGE, GLenum mn = GL_LINEAR, GLenum mg = GL_LINEAR) {
	glTexParameteri(T, GL_TEXTURE_MAX_LEVEL, mp);
	glTexParameteri(T, GL_TEXTURE_WRAP_S, ws);
	glTexParameteri(T, GL_TEXTURE_WRAP_T, wt);
	glTexParameteri(T, GL_TEXTURE_MIN_FILTER, mn);
	glTexParameteri(T, GL_TEXTURE_MAG_FILTER, mg);
}

class Texture_I {
public:
    static void FlipY(std::vector<byte>& data, uint w, uint h);

    static bool FromJPG(const std::string& path, uint& w, uint& h, byte& channels, std::vector<byte>& data);

    static bool FromPNG(const std::string& path, uint& w, uint& h, byte& channels, std::vector<byte>& data);

    static bool FromBMP(const std::string& path, uint& w, uint& h, byte& channels, std::vector<byte>& data);
};

CE_END_NAMESPACE