#include "chokoengine.hpp"
#include "texture_internal.hpp"

CE_BEGIN_NAMESPACE

_Texture::_Texture(std::nullptr_t)
        : _pointer(0), _width(0), _height(0), _hdr(false) {}

_Texture::_Texture(uint w, uint h, bool hdr)
		: _width(w), _height(h), _channels(4), _hdr(hdr) {
	glGenTextures(1, &_pointer);
	glBindTexture(GL_TEXTURE_2D, _pointer);
	std::vector<byte> data(w * h * 4);
	glTexImage2D(GL_TEXTURE_2D, 0, hdr? GL_RGBA32F : GL_RGBA, (int)w, (int)h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
	SetTexParams<>(0, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
		GL_LINEAR, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

_Texture::_Texture(const std::string& path, const TextureOptions& opts) : _hdr(false) {
    std::string ss = path.substr(path.find_last_of('.') + 1, std::string::npos);

    std::vector<byte> data;
    GLenum rgb = GL_RGB, rgba = GL_RGBA;

    if (ss == "jpg") {
        if (!Texture_I::FromJPG(path, _width, _height, _channels, data))
            return;
    }
    else if (ss == "png") {
        if (!Texture_I::FromPNG(path, _width, _height, _channels, data))
            return;
        rgb = GL_BGR;
        rgba = GL_BGRA;
    }
    else if (ss == "bmp") {
        if (!Texture_I::FromBMP(path, _width, _height, _channels, data))
            return;
    }
	else if (ss == "hdr") {
		if (!Texture_I::FromHDR(path, _width, _height, _channels, data))
			return;
		_hdr = true;
	}
    else {
        Debug::Error("Texture", "Cannot determine format of \"" + path + "\"!");
        return;
    }

    glGenTextures(1, &_pointer);
	glBindTexture(GL_TEXTURE_2D, _pointer);
	if (_channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, _hdr ? GL_RGB32F : GL_RGB, _width, _height, 0, rgb, _hdr ? GL_FLOAT : GL_UNSIGNED_BYTE, data.data());
	else
        glTexImage2D(GL_TEXTURE_2D, 0, _hdr ? GL_RGBA32F : GL_RGBA, _width, _height, 0, rgba, _hdr ? GL_FLOAT : GL_UNSIGNED_BYTE, data.data());
	if (opts.mipmaps > 0) glGenerateMipmap(GL_TEXTURE_2D);
    const GLenum wraps[] = { GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT };
    SetTexParams<>(0,
        wraps[(int)opts.xwrap],
        wraps[(int)opts.ywrap],
		(opts.linear) ? (
            (opts.mipmaps > 0) ?
                GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR
        ) : GL_NEAREST,
        (opts.linear) ?
            GL_LINEAR : GL_NEAREST
    );
	glBindTexture(GL_TEXTURE_2D, 0);
}

_Texture::~_Texture() {
	glDeleteTextures(1, &_pointer);
}

bool _Texture::loaded() const {
    return !!_pointer;
}

void _Texture::Bind() const {
	glBindTexture(GL_TEXTURE_2D, _pointer);
}

void _Texture::Unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

CE_END_NAMESPACE