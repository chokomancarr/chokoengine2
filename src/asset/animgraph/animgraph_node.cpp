#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_AnimGraph::Node::Node() : _clip(nullptr), _length(0) {}

void _AnimGraph::Node::clip(const AnimClip& c) {
	if (!!(_clip = c)) {
		_length = c->range().y - c->range().x;
	}
	else {
		_length = 0;
	}
}

CE_END_NAMESPACE