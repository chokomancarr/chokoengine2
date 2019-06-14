#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE
namespace glext {

template <typename T, GLenum E = GL_ARRAY_BUFFER>
void SetBuf(GLuint buf, T* data, GLsizeiptr sz, GLenum st = GL_STATIC_DRAW);

template <typename T, GLenum E = GL_ARRAY_BUFFER>
void SetSubBuf(GLuint buf, T* data, GLsizeiptr sz);

}
CE_END_NAMESPACE

#include "detail/glext.inl"