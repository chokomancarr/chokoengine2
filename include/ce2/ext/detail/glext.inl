#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE
namespace glext {

template <typename T, GLenum E>
void SetBuf(GLuint buf, T* data, GLsizeiptr sz, GLenum st) {
    glBindBuffer(E, buf);
	glBufferData(E, sz * sizeof(T), data, st);
    glBindBuffer(E, 0);
}

template <typename T, GLenum E>
void SetSubBuf(GLuint buf, T* data, GLsizeiptr sz) {
    glBindBuffer(E, buf);
	glBufferSubData(E, 0, sz * sizeof(T), data);
    glBindBuffer(E, 0);
}

}
CE_END_NAMESPACE