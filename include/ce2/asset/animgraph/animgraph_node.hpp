#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _AnimGraph::_Node : public _Object {
	AnimClip _clip;

	std::vector<Link> _links;

public:
	_Node();

	CE_GET_SET_MEMBER(clip);
	CE_GET_SET_MEMBER(links);
};

CE_END_NAMESPACE