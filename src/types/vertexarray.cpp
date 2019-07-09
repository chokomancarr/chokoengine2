#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_VertexArray::_VertexArray() : _buffers({}), _bufferSize(0) {
    glGenVertexArrays(1, &_pointer);
}

_VertexArray::~_VertexArray() {
	glDeleteVertexArrays(1, &_pointer);
}

const VertexBuffer& _VertexArray::buffer(int i) const {
    return _buffers[i];
}

void _VertexArray::AddBuffer(const VertexBuffer& buf, int div) {
    Bind();
    glEnableVertexAttribArray(_bufferSize);
    buf->Bind();
    if (buf->isfloat())
        glVertexAttribPointer(_bufferSize, buf->dim(), GL_FLOAT, GL_FALSE, 0, 0);
    else
        glVertexAttribIPointer(_bufferSize, buf->dim(), GL_INT, 0, 0);
    glVertexAttribDivisor(_bufferSize, div);
    buf->Unbind();
    Unbind();
    _buffers.push_back(buf);
    _bufferSize++;
}

void _VertexArray::Bind() const {
    glBindVertexArray(_pointer);
}

void _VertexArray::Unbind() const {
    glBindVertexArray(0);
}

VertexArray VertexArray_New() {
    return std::make_shared<_VertexArray>();
}

CE_END_NAMESPACE