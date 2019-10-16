#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* Render Target object used for off-screen rendering
 */
class _RenderTarget3 : public _Texture3 { CE_OBJECT_COMMON
	GLuint _fbo;

public:
	_RenderTarget3(uint w, uint h, uint d, GLenum fmt);

	~_RenderTarget3();

	void BindTarget() const;
	void UnbindTarget() const;
};

CE_END_NAMESPACE