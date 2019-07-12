#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _AnimGraph::_Link : public _Object {
	pNode _target;

public:
	_Link();
};

CE_END_NAMESPACE