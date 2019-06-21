#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Texture : public _Asset { CE_OBJECT_COMMON
protected: //allow rendertarget access
	_Texture(uint w, uint h, bool hdr);

    GLuint _pointer;

    uint _width, _height;
    byte _channels;
    bool _hdr;

	bool partialScaling;
	Rect scalingArea;

public:
	_Texture();

    /* Loads an image from the specified path
     * Supported formats: png, jpg, bmp, hdr
     */
	_Texture(const std::string& path);

    virtual ~_Texture() = default; //allow render targets etc to override

    CE_GET_MEMBER(width);
    CE_GET_MEMBER(height);
	CE_GET_MEMBER(channels);
	CE_GET_MEMBER(hdr);

	bool loaded() const;

    friend class UI;
};

CE_END_NAMESPACE