#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_VertexBuffer::_VertexBuffer(bool isf, size_t dim, size_t num, void* data, size_t stride, GLenum type, GLenum usage)
        : _isfloat(isf), _dim(dim), _num(num), _type(type) {
    glGenBuffers(1, &_pointer);
    glBindBuffer(type, _pointer);
    glBufferData(type, num * dim * 4, data, usage);
    glBindBuffer(type, 0);
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


_VertexObject::_VertexObject() {
    glGenVertexArrays(1, &_pointer);
}

const VertexBuffer& _VertexObject::buffer(int i) const {
    return _buffers[i];
}

void _VertexObject::AddBuffer(const VertexBuffer& buf) {
    Bind();
    glEnableVertexAttribArray(_bufferSize);
    buf->Bind();
    if (buf->isfloat())
        glVertexAttribPointer(_bufferSize, buf->dim(), GL_FLOAT, GL_FALSE, 0, 0);
    else
        glVertexAttribIPointer(_bufferSize, buf->dim(), GL_INT, 0, 0);
    buf->Unbind();
    Unbind();
    _buffers.push_back(buf);
    _bufferSize++;
}

void _VertexObject::Bind() const {
    glBindVertexArray(_pointer);
}

void _VertexObject::Unbind() const {
    glBindVertexArray(0);
}

VertexObject VertexObject_New() {
    return std::make_shared<_VertexObject>();
}

CE_END_NAMESPACE