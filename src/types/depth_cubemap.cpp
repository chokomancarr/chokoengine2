#include "chokoengine.hpp"
#include "asset/texture_internal.hpp"

CE_BEGIN_NAMESPACE

_DepthCubeMap::_DepthCubeMap(uint r) : _reso(r) {
	glGenTextures(1, &_pointer);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _pointer);
	for (int a = 0; a < 6; a++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + a, 0, GL_DEPTH_COMPONENT24, (int)r, (int)r, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		SetTexParams<>(0, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

DepthCubeMap DepthCubeMap_New(uint r) {
	return std::make_shared<_DepthCubeMap>(r);
}

CE_END_NAMESPACE