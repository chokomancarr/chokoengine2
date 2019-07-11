#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _AnimClipEntry { CE_OBJECT_COMMON
	std::string signature;
	

public:
	_AnimClipEntry(string name, AnimKeyType type, uint cnt = 0, float* loc = 0) : name(name), type(type),
		dim((type == AK_ShapeKey) ? 1 : ((type & 0x0f) == 0x01) ? 4 : 3) {
		if (!!cnt) AddFrames(cnt, loc);
	}

	std::vector<std::pair<int, Vec4>> frames;
	uint frameCount;

	string name;
	const AnimKeyType type;
	const byte dim;

	void AddFrames(uint cnt, float* loc);
	Vec4 Eval(float t);
};

CE_END_NAMESPACE