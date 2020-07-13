#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_AnimGraph::_AnimGraph() : _Asset(AssetType::AnimGraph), _nodes({}), _vars({}), defaultState(0) {}

_AnimGraph::Node& _AnimGraph::AddNode() {
    _nodes.push_back(Node::New());
	_nodes.back()->name("new state");
    return _nodes.back();
}

_AnimGraph::State _AnimGraph::GetNewState() {
	if (!_nodes.size()) {
		Debug::Warning("AnimGraph::GetNewState", "no states in current graph!");
	}
    State s = {};
    s.inTransition = false;
    s.node = _nodes.front();
    s.link = 0;
    for (auto& v : _vars) {
        s.vars.push_back(v.defaultValue);
    }
    return s;
}

void _AnimGraph::Update(State& st) const {
    
}

_AnimClip::VQ _AnimGraph::Get(const std::string& sig) const {
	return _nodes[0]->clip()->Get(sig, std::fmod(Time::time(), 1.f) * 24);
}

CE_END_NAMESPACE