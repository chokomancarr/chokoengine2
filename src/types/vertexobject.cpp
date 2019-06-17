#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_VertexObject::_VertexObject() : _buffers({}), _bufferSize(0) {
    glGenVertexArrays(1, &_pointer);
}

_VertexObject::~_VertexObject() {
	glDeleteVertexArrays(1, &_pointer);
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