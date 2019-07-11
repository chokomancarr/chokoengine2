#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _AnimGraph : public _Asset { CE_OBJECT_COMMON
public:
	CE_OBJECT(State);
	CE_OBJECT(Transition);

private:
	std::vector<State> states;
	std::vector<Transition> transitions;

public:
	_AnimGraph();
};

CE_END_NAMESPACE

#include "animgraph/animgraph_state.hpp"
#include "animgraph/animgraph_transition.hpp"