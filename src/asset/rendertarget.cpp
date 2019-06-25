#include "chokoengine.hpp"
#include "texture_internal.hpp"
#include "backend/rend/renderer.hpp"

CE_BEGIN_NAMESPACE

_RenderTarget::_RenderTarget() : _Texture(nullptr) {}

_RenderTarget::_RenderTarget(uint w, uint h, bool hdr)
		: _Texture(w, h, hdr) {
	glGenTextures(1, &_depth);
	glBindTexture(GL_TEXTURE_2D, _depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, (int)w, (int)h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	SetTexParams<>(0, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _pointer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth, 0);

	GLenum dbufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(1, dbufs);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Debug::Error("Render Target", "Could not create internal framebuffer: gl error " + std::to_string(status));
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void _RenderTarget::Blit(const Texture& src, const RenderTarget& dst, const Material& mat) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst->_fbo);
	if (!!src) {
		mat->SetUniform("mainTex", src);
	}
	mat->Bind();
	Backend::Renderer::emptyVao()->Bind();
	glViewport(0, 0, dst->_width, dst->_height);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	Backend::Renderer::emptyVao()->Unbind();
	mat->Unbind();
	glViewport(0, 0, Display::width(), Display::height());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

CE_END_NAMESPACE