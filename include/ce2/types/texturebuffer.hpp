#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _TextureBuffer {
	GLuint _pointer;
	VertexBuffer _buffer;

	_TextureBuffer(const _VertexObject&) = delete;
public:
	_TextureBuffer() = default;
	_TextureBuffer(const VertexBuffer& buf, GLenum fmt);
	~_TextureBuffer();

	CE_GET_MEMBER(pointer);
	CE_GET_MEMBER(buffer);

	void Bind() const;
	void Unbind() const;
};

typedef std::shared_ptr<_TextureBuffer> TextureBuffer;
TextureBuffer TextureBuffer_New(const VertexBuffer& buf, GLenum fmt);

CE_END_NAMESPACE