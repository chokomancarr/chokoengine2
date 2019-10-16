#include "chokoengine.hpp"
#include "texture_internal.hpp"
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_NAMESPACE

_Texture3::_Texture3(std::nullptr_t)
		: _pointer(0), _width(0), _height(0), _depth(0), _format(0) {}

_Texture3::_Texture3(uint w, uint h, uint d, GLenum fmt)
		: _width(w), _height(h), _depth(d), _channels(4), _format(fmt) {
	glGenTextures(1, &_pointer);
	glBindTexture(GL_TEXTURE_3D, _pointer);
	std::vector<byte> data(w * h * d * 4);
	glTexImage3D(GL_TEXTURE_3D, 0, fmt, (int)w, (int)h, (int)d, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
	SetTexParams<GL_TEXTURE_3D>(0, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
	glBindTexture(GL_TEXTURE_3D, 0);
	
	if (glGetError() != 0) {
		Debug::Error("Texture3", "OpenGL Error detected: Is the format " + std::to_string(fmt) + " supported?");
	}
}

_Texture3::~_Texture3() {
	glDeleteTextures(1, &_pointer);
}

bool _Texture3::loaded() const {
	return !!_pointer;
}

void _Texture3::Bind() const {
	glBindTexture(GL_TEXTURE_3D, _pointer);
}

void _Texture3::Unbind() const {
	glBindTexture(GL_TEXTURE_3D, 0);
}

CE_END_NAMESPACE