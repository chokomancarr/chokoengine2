#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

class _Texture { CE_OBJECT_COMMON
    GLuint _pointer;

    uint _width, _height;
    byte _channels;
    bool _hdr;

public:
	_Texture();

    /* Loads an image from the specified path
     * Supported formats: png, jpg, bmp, hdr
     */
	_Texture(const std::string& path);

    GET_MEMBER_FUNC(int, width);
    GET_MEMBER_FUNC(int, height);

	bool loaded() const;

    friend class UI;
};

END_CE_NAMESPACE