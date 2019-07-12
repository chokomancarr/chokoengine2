#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _AnimGraph : public _Asset { CE_OBJECT_COMMON
public:
	CE_OBJECT(Node);
	CE_OBJECT(Link);

	class _Var;
	class Var;

	/* Object containing the current state of the machine
	 * Required for every update operation
	 */
	struct State;

private:
	std::vector<Node> _nodes;
	std::vector<Var> _vars;

public:
	_AnimGraph();

	CE_GET_SET_MEMBER(nodes);

	Node& AddNode();

	State GetNewState();

	void Update(State&) const;

	_AnimClip::Value Get(const std::string& sig) const;
};

CE_END_NAMESPACE

#include "animgraph/animgraph_node.hpp"
#include "animgraph/animgraph_link.hpp"
#include "animgraph/animgraph_var.hpp"
#include "animgraph/animgraph_state.hpp"