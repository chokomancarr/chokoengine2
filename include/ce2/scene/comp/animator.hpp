#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Animator : public _Component { CE_COMPONENT_COMMON
    AnimGraph _graph;

    _AnimGraph::State _state;

public:
	_Animator();

	/* The animation graph
	*/
    CE_GET_SET_MEMBER_F(graph);

	void OnUpdate() override;
};

CE_END_NAMESPACE