#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

struct _AnimGraph::State {
	const Node* node_current, *node_target;
	float transition_time; //-1 for not transitioning
	float transition_max;
	float time_current, time_target;
	std::vector<_Var> vars;
};

CE_END_NAMESPACE