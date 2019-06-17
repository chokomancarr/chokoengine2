#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_VertexBuffer::_VertexBuffer(bool isf, size_t dim, size_t num, void* data, size_t stride, GLenum type, GLenum usage)
	: _isfloat(isf), _dim(dim), _num(num), _type(type) {
	glGenBuffers(1, &_pointer);
	glBindBuffer(type, _pointer);
	glBufferData(type, num * dim * 4, data, usage);
	glBindBuffer(type, 0);
}

_VertexBuffer::~_VertexBuffer() {
	glDeleteBuffers(1, &_pointer);
}

void _VertexBuffer::Set(void* data, size_t len) {
	Bind();
	glBufferSubData(_type, 0, len * _dim * 4, data);
	Unbind();
}

void _VertexBuffer::Bind() const {
	glBindBuffer(_type, _pointer);
}

void _VertexBuffer::Unbind() const {
	glBindBuffer(_type, 0);
}

VertexBuffer VertexBuffer_New(bool isf, size_t dim, size_t num, void* data, size_t stride, GLenum type, GLenum usage) {
	return std::make_shared<_VertexBuffer>(isf, dim, num, data, stride, type, usage);
}

CE_END_NAMESPACE