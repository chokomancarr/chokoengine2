#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _AnimGraph::_State {
	AnimClip _clip;

public:
	_State();

	CE_GET_SET_MEMBER(clip);
};

CE_END_NAMESPACE