#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <typename T>
std::vector<T> _VertexBuffer::Get() {
	const auto numbytes = _dim * _num * 4;
	assert((numbytes / sizeof(T)) * sizeof(T) == numbytes);

	std::vector<T> res(numbytes / sizeof(T));
	_Get(res.data());

	return res;
}

CE_END_NAMESPACE