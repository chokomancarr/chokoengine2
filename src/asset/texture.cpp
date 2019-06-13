#include "chokoengine.hpp"
#include "texture_internal.hpp"

BEGIN_CE_NAMESPACE

_Texture::_Texture()
        : _pointer(0), _width(0), _height(0), _hdr(false) {}

_Texture::_Texture(const std::string& path) : _pointer(0) {
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
    else {
        Debug::Error("Texture", "Cannot determine format of \"" + path + "\"!");
        return;
    }

    glGenTextures(1, &_pointer);
	glBindTexture(GL_TEXTURE_2D, _pointer);
	if (_channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, rgb, GL_UNSIGNED_BYTE, data.data());
	else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, rgba, GL_UNSIGNED_BYTE, data.data());
	glGenerateMipmap(GL_TEXTURE_2D);
    SetTexParams<>(0, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
		GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool _Texture::loaded() const {
    return !!_pointer;
}

END_CE_NAMESPACE