#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_TextureBuffer::_TextureBuffer(const VertexBuffer& buf, GLenum fmt) : _Texture(nullptr) {
	glGenTextures(1, &_pointer);
	glBindTexture(GL_TEXTURE_BUFFER, _pointer);
	glTexBuffer(GL_TEXTURE_BUFFER, fmt, buf->pointer());
	glBindTexture(GL_TEXTURE_BUFFER, 0);
}

void _TextureBuffer::Bind() const {
	glBindTexture(GL_TEXTURE_BUFFER, _pointer);
}

void _TextureBuffer::Unbind() const {
	glBindTexture(GL_TEXTURE_BUFFER, 0);
}

CE_END_NAMESPACE