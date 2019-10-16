#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Texture3 : public _Asset { CE_OBJECT_COMMON
protected: //allow rendertarget access
    _Texture3(std::nullptr_t);

    GLuint _pointer;

    uint _width, _height, _depth;
    byte _channels;
    GLenum _format;

public:
	_Texture3(uint w, uint h, uint d, GLenum fmt);

    virtual ~_Texture3(); //allow render targets etc to override

    CE_GET_MEMBER(width);
    CE_GET_MEMBER(height);
	CE_GET_MEMBER(depth);
	CE_GET_MEMBER(channels);
	CE_GET_MEMBER(format);

	bool loaded() const;

    virtual void Bind() const;
    virtual void Unbind() const;
};

CE_END_NAMESPACE