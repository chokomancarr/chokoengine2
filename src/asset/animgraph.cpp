#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_AnimGraph::_AnimGraph() : _Asset(AssetType::AnimGraph), _nodes({}), _vars({}) {}

_AnimGraph::Node& _AnimGraph::AddNode() {
    _nodes.push_back(Node::New());
    return _nodes.back();
}

_AnimGraph::State _AnimGraph::GetNewState() {
    State s = {};
    s.inTransition = false;
    s.node = _nodes.front();
    s.link = pLink();
    for (auto& v : _vars) {
        s.vars.push_back(v.defaultValue);
    }
    return s;
}

void _AnimGraph::Update(State& st) const {
    
}

_AnimClip::Value _AnimGraph::Get(const std::string& sig) const {
    auto& ee = _nodes[0]->clip()->entries();
    auto it = std::find_if(ee.begin(), ee.end(), [&](const _AnimClip::Entry& e) {
        return e.signature == sig;
    });
    if (it == ee.end()) {
        return _AnimClip::Value();
    }
    return it->values.Eval(Time::time());
}

CE_END_NAMESPACE