#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE
namespace glext {

template <typename T, GLenum E = GL_ARRAY_BUFFER>
void SetBuf(GLuint buf, T* data, GLsizeiptr sz, GLenum st = GL_STATIC_DRAW);

template <typename T, GLenum E = GL_ARRAY_BUFFER>
void SetSubBuf(GLuint buf, T* data, GLsizeiptr sz);

}
END_CE_NAMESPACE

#include "detail/glext.inl"