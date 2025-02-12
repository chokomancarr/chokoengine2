#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _VertexBuffer {
	_VertexBuffer(const _VertexBuffer&) = delete;

	GLuint _pointer;
	bool _isfloat;
	size_t _dim;
	size_t _num;
	GLenum _type;
	GLenum _usage;
public:
	_VertexBuffer() = default;
	_VertexBuffer(bool isf, size_t dim, size_t num, const void* data, size_t stride = 0, GLenum type = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_COPY);
	~_VertexBuffer();

	CE_GET_MEMBER(pointer);
	CE_GET_MEMBER(isfloat);
	CE_GET_MEMBER(dim);
	CE_GET_MEMBER(num);
	CE_GET_MEMBER(type);

	void Set(const void* data, size_t len);

	template <typename T>
	std::vector<T> Get();

	void Bind() const;
	void Unbind() const;

	friend class _TransformFeedback;

private:
	void _Get(void* const buf);
};

VertexBuffer VertexBuffer_New(bool isf, size_t dim, size_t num, const void* data, size_t stride = 0, GLenum type = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_COPY);

CE_END_NAMESPACE

#include "detail/vertexbuffer.inl"