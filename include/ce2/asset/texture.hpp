#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Texture { CE_OBJECT_COMMON
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

    CE_GET_MEMBER(width);
    CE_GET_MEMBER(height);

	bool loaded() const;

    friend class UI;
};

CE_END_NAMESPACE