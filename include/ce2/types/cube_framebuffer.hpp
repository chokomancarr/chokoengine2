#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _FrameBufferCube {CE_OBJECT_COMMON
	std::array<RenderTarget, 6> _pointers;
	RenderTarget _tmpPointer;
	std::vector<CubeMap> _maps;
	DepthCubeMap _depth;

	uint64_t _lastUpdated;

	_FrameBufferCube(const _FrameBufferCube&) = delete;
public:
	_FrameBufferCube(uint r, std::vector<GLenum> types, int div = 0);

	const CubeMap& map(int i);
	CE_GET_MEMBER(depth);

	void Clear() const;
	void Bind(CubeMapFace f) const;
	void Unbind() const;
};

typedef std::shared_ptr<_FrameBufferCube> FrameBufferCube;
FrameBufferCube FrameBufferCube_New(uint r, std::vector<GLenum> types);

CE_END_NAMESPACE