#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _DepthTexture { CE_OBJECT_COMMON
    GLuint _pointer;

    uint _width, _height;

public:
	_DepthTexture(uint w, uint h);

	virtual ~_DepthTexture();

    CE_GET_MEMBER(width);
    CE_GET_MEMBER(height);

	friend class _FrameBuffer;
};

typedef std::shared_ptr<_DepthTexture> DepthTexture;
DepthTexture DepthTexture_New(uint w, uint h);

CE_END_NAMESPACE