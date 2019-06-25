#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _RenderTarget : public _Texture { CE_OBJECT_COMMON
	GLuint _depth;
	GLuint _fbo;

public:
	_RenderTarget();
	_RenderTarget(uint w, uint h, bool hdr);

	CE_GET_MEMBER(fbo);

	/* Copies a texture to the destination render target
	 * The texture will be set as variable mainTex of material,
	 * or executed directly if null is provided
	 */
	static void Blit(const Texture& src, const RenderTarget& dst, const Material& mat);
};

CE_END_NAMESPACE