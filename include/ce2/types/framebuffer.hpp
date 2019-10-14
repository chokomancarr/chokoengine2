#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _FrameBuffer {CE_OBJECT_COMMON
	GLuint _pointer;
	std::vector<Texture> _texs;
	DepthTexture _depth;

	uint64_t _lastUpdated;

	_FrameBuffer(const _FrameBuffer&) = delete;
public:
	_FrameBuffer(uint w, uint h, std::vector<GLenum> types);
	~_FrameBuffer();

	CE_GET_MEMBER(pointer);
	const Texture& tex(int i);
	CE_GET_MEMBER(depth);

	void Clear() const;
	void Bind() const;
	void Unbind() const;
};

typedef std::shared_ptr<_FrameBuffer> FrameBuffer;
FrameBuffer FrameBuffer_New(uint w, uint h, std::vector<GLenum> types);

CE_END_NAMESPACE