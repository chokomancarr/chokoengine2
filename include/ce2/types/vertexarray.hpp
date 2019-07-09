#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _VertexArray { CE_CLASS_COMMON
	GLuint _pointer;
	std::vector<VertexBuffer> _buffers;
	int _bufferSize;

	_VertexArray(const _VertexArray&) = delete;
public:
	_VertexArray();
	~_VertexArray();

	CE_GET_MEMBER(pointer);
	CE_GET_MEMBER(buffers);
	const VertexBuffer& buffer(int i) const;
	CE_GET_MEMBER(bufferSize);

	void AddBuffer(const VertexBuffer& buf, int div = 0);
	void Bind() const;
	void Unbind() const;
};

VertexArray VertexArray_New();

CE_END_NAMESPACE