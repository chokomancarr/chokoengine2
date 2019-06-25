#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _TextureBuffer : public _Texture { CE_OBJECT_COMMON
	VertexBuffer _buffer;

	_TextureBuffer(const _VertexObject&) = delete;
public:
	_TextureBuffer(const VertexBuffer& buf, GLenum fmt);

	CE_GET_MEMBER(buffer);

	void Bind() const override;
	void Unbind() const override;
};

TextureBuffer TextureBuffer_New(const VertexBuffer& buf, GLenum fmt);

CE_END_NAMESPACE