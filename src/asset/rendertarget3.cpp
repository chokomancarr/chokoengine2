#include "chokoengine.hpp"
#include "texture_internal.hpp"
#include "backend/rend/renderer.hpp"

CE_BEGIN_NAMESPACE

_RenderTarget3::_RenderTarget3(uint w, uint h, uint d, GLenum fmt)
		: _Texture3(w, h, d, fmt) {

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _pointer, 0);

	GLenum dbuf = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &dbuf);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Debug::Error("Render Target", "Could not create 3d framebuffer: gl error " + std::to_string(status));
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

_RenderTarget3::~_RenderTarget3() {
	glDeleteFramebuffers(1, &_fbo);
}

void _RenderTarget3::BindTarget() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
}

void _RenderTarget3::UnbindTarget() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

CE_END_NAMESPACE