#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _DepthCubeMap { CE_OBJECT_COMMON
	GLuint _pointer;

	uint _reso;

public:
	_DepthCubeMap(uint r);

	CE_GET_MEMBER(reso);

	friend class _FrameBufferCube;
};

typedef std::shared_ptr<_DepthCubeMap> DepthCubeMap;
DepthCubeMap DepthCubeMap_New(uint r);

CE_END_NAMESPACE