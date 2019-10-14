#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Texture3 : public _Asset { CE_OBJECT_COMMON
protected: //allow rendertarget access
    _Texture3(std::nullptr_t);

    GLuint _pointer;

    uint _width, _height, _depth;
    byte _channels;
    bool _hdr;

public:
	_Texture3(uint w, uint h, uint d, bool hdr);

    virtual ~_Texture3(); //allow render targets etc to override

    CE_GET_MEMBER(width);
    CE_GET_MEMBER(height);
	CE_GET_MEMBER(depth);
	CE_GET_MEMBER(channels);
	CE_GET_MEMBER(hdr);

	bool loaded() const;

    virtual void Bind() const;
    virtual void Unbind() const;
};

CE_END_NAMESPACE