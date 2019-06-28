#include "chokoengine.hpp"
#include "texture_internal.hpp"
#include "backend/rend/renderer.hpp"

CE_BEGIN_NAMESPACE

_RenderTarget::_RenderTarget(uint w, uint h, bool hdr, bool d)
		: _Texture(w, h, hdr), _depth(0) {
	if (d) {
		glGenTextures(1, &_depth);
		glBindTexture(GL_TEXTURE_2D, _depth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, (int)w, (int)h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		SetTexParams<>(0, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _pointer, 0);
	if (d) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth, 0);

	GLenum dbuf = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &dbuf);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Debug::Error("Render Target", "Could not create framebuffer: gl error " + std::to_string(status));
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void _RenderTarget::BindTarget() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
}

void _RenderTarget::UnbindTarget() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

CE_END_NAMESPACE