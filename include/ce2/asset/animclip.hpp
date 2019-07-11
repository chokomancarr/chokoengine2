#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _AnimClip : public _Asset { CE_OBJECT_COMMON
public:
	std::vector<AnimClip_Key*> keys;
	ushort keyCount;
	ushort frameStart, frameEnd;
	bool repeat;

	std::vector<string> keynames;
	std::vector<Vec4> keyvals;

	Interpolation interpolation;

	void Eval(float t);

	AnimClip(Editor* e, int i);
	AnimClip(std::ifstream& strm, uint offset);
	AnimClip(string path);
};

CE_END_NAMESPACE