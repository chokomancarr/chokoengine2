#pragma once
#include "chokoengine.hpp"
#include "texture/texture_options.hpp"

CE_BEGIN_NAMESPACE

class _Texture : public _Asset { CE_OBJECT_COMMON CE_OBJECT_ALLOW_ASYNC
protected: //allow rendertarget access
    _Texture(std::nullptr_t);
    _Texture(uint w, uint h, GLuint ptr);

    GLuint _pointer;

    uint _width, _height;
    byte _channels;
    bool _hdr;

	bool partialScaling;
	Rect scalingArea;

	const TextureOptions _opts;
	std::vector<byte> _pixels;

public:
	_Texture(uint w, uint h, bool hdr, const TextureOptions& opts);

	_Texture(uint w, uint h, GLenum type, const TextureOptions& opts, const void* pixels = nullptr, const GLenum pixelFmt = GL_RGBA, const GLenum pixelType = GL_UNSIGNED_BYTE);

    /* Loads an image from the specified path
     * Supported formats: png, jpg, bmp, hdr
     */
	_Texture(const std::string& path, const TextureOptions& opts = TextureOptions(), bool async = false);

    virtual ~_Texture(); //allow render targets etc to override

    CE_GET_MEMBER(width);
    CE_GET_MEMBER(height);
	CE_GET_MEMBER(channels);
	CE_GET_MEMBER(hdr);

    void SetPixelsRaw(const std::vector<byte>&);
	void SetPixelsRaw(const std::vector<float>&);

    virtual void Bind();
    virtual void Unbind() const;

	/* Copies the texture to the destination render target, applying the material
     * If the material is null, the texture is copied directly
	 */
	void Blit(const RenderTarget& dst, const Material& mat);

    friend class UI;
	friend class _Material;
	friend class _FrameBuffer;
};

CE_END_NAMESPACE