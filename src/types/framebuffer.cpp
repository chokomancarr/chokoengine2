#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_FrameBuffer::_FrameBuffer(uint w, uint h, std::vector<GLenum> types) 
		: _texs(types.size(), nullptr), _depth(nullptr), _lastUpdated(0) {
	std::vector<GLenum> bufs(types.size());
	glGenFramebuffers(1, &_pointer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _pointer);

	for (size_t a = 0; a < types.size(); a++) {
		_texs[a] = Texture::New(w, h, types[a], TextureOptions(
			TextureWrap::Clamp, TextureWrap::Repeat, 0, false
		));
		glFramebufferTexture2D(GL_FRAMEBUFFER, (bufs[a] = GL_COLOR_ATTACHMENT0 + (GLsizei)a), GL_TEXTURE_2D, _texs[a]->_pointer, 0);
	}
	_depth = DepthTexture_New(w, h);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth->_pointer, 0);

	glDrawBuffers((GLsizei)bufs.size(), bufs.data());
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Debug::Error("FrameBuffer", "gl error " + std::to_string(status));
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

_FrameBuffer::~_FrameBuffer() {
	glDeleteFramebuffers(1, &_pointer);
}

void _FrameBuffer::Clear() const {
	float zeros[4] = { 1, 0, 0, 1 };
	float one = 1;
	for (int a = 0; a < _texs.size(); a++) {
		glClearBufferfv(GL_COLOR, a, zeros);
	}
	glClearBufferfv(GL_DEPTH, 0, &one);
}

void _FrameBuffer::Bind() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _pointer);
}

void _FrameBuffer::Unbind() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

FrameBuffer FrameBuffer_New(uint w, uint h, std::vector<GLenum> types) {
	return std::make_shared<_FrameBuffer>(w, h, types);
}

CE_END_NAMESPACE