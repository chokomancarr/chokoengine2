#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _RenderTarget : public _Texture { CE_OBJECT_COMMON
	GLuint _depth;
	GLuint _fbo;

public:
	_RenderTarget();
	_RenderTarget(uint w, uint h, bool hdr);
};

CE_END_NAMESPACE