#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _AnimGraph::Node {

	AnimClip _clip;

	float _length;

public:
	Node();

	std::string name;

	CE_GET_SET_MEMBER_F(clip);

	CE_GET_MEMBER(length);

	float speed;
	bool repeat;

	std::vector<Link> links;
};

CE_END_NAMESPACE