#include "chokoengine.hpp"
#include "asset/texture_internal.hpp"

CE_BEGIN_NAMESPACE

_DepthTexture::_DepthTexture(uint w, uint h) : _width(w), _height(h) {
	glGenTextures(1, &_pointer);
	glBindTexture(GL_TEXTURE_2D, _pointer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, (int)w, (int)h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	SetTexParams<>(0, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

_DepthTexture::~_DepthTexture() {
	glDeleteTextures(1, &_pointer);
}

DepthTexture DepthTexture_New(uint w, uint h) {
	return std::make_shared<_DepthTexture>(w, h);
}

CE_END_NAMESPACE