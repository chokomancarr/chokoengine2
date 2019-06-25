#pragma once
#include "chokoengine.hpp"
#include "enums/texture_wrap.hpp"
#include "texture/texture_options.hpp"

CE_BEGIN_NAMESPACE

class _Texture : public _Asset { CE_OBJECT_COMMON
protected: //allow rendertarget access
    _Texture(std::nullptr_t);

	_Texture(uint w, uint h, GLenum type, const TextureOptions& opts);

    GLuint _pointer;

    uint _width, _height;
    byte _channels;
    bool _hdr;

	bool partialScaling;
	Rect scalingArea;

public:
	_Texture(uint w, uint h, bool hdr);

    /* Loads an image from the specified path
     * Supported formats: png, jpg, bmp, hdr
     */
	_Texture(const std::string& path, const TextureOptions& opts = TextureOptions());

    virtual ~_Texture(); //allow render targets etc to override

    CE_GET_MEMBER(width);
    CE_GET_MEMBER(height);
	CE_GET_MEMBER(channels);
	CE_GET_MEMBER(hdr);

	bool loaded() const;

    virtual void Bind() const;
    virtual void Unbind() const;

    friend class UI;
	friend class _Material;
	friend class _FrameBuffer;
};

CE_END_NAMESPACE