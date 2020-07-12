#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

struct _AnimGraph::State {
	bool inTransition;
	pNode node;
	int link;
	std::vector<_Var> vars;
};

CE_END_NAMESPACE