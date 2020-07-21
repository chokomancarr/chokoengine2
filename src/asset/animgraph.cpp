#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

#define FRAMERATE 24
#define TIMERATE Time::delta() * FRAMERATE

_AnimGraph::_AnimGraph() : _Asset(AssetType::AnimGraph), _nodes({}), _vars({}), defaultState(0) {}

_AnimGraph::Node& _AnimGraph::AddNode() {
    _nodes.push_back(Node());
    return _nodes.back();
}

_AnimGraph::State _AnimGraph::GetNewState() const {
	if (!_nodes.size()) {
		Debug::Warning("AnimGraph::GetNewState", "no states in animation graph!");
	}
    State s = {};
	s.node_current = &_nodes[defaultState];
	s.transition_time = -1;
	s.time_current = 0;
	s.vars.reserve(_vars.size());
    for (auto& v : _vars) {
        s.vars.push_back(v.defaultValue);
    }
    return s;
}

void _AnimGraph::Update(State& st) const {
	if (st.transition_time < 0) {
		for (const auto& tr : st.node_current->links) {
			for (auto& cnds : tr.conditions) {
				for (auto& cnd : cnds) {
					if (!cnd.Check(*this, st)) {
						goto nosat;
					}
				}

				if (tr.useExitLength) {
					const auto len = st.node_current->length();
					if (len - st.time_current > tr.exitLength * FRAMERATE) {
						goto nosat;
					}
				}
				
				st.node_target = &_nodes[tr.target];
				st.transition_time = 0;
				st.transition_max = tr.length;
				st.time_target = tr.offset;
				goto foundsat;
			nosat:
				continue;
			}
		}
	foundsat:;
	}
	else {
		if ((st.transition_time += Time::delta()) >= st.transition_max) {
			st.transition_time = -1;
			st.time_current = st.time_target;
			st.node_current = st.node_target;
		}
		else {
			const auto len = st.node_target->length();
			if ((st.time_target += TIMERATE * st.node_target->speed) > len && st.node_target->repeat) {
				st.time_target -= len;
			}
		}
	}

	const auto len = st.node_current->length();
	if ((st.time_current += TIMERATE * st.node_current->speed) > len && st.node_current->repeat) {
		st.time_current -= len;
	}
}

_AnimClip::VQ _AnimGraph::Get(const State& st, const std::string& sig) const {
	const auto cur = st.node_current->Get(st, sig, st.time_current);

	if (st.transition_time < 0) return cur;

	const auto tar = st.node_target->Get(st, sig, st.time_target);
	
	return cur.Blend(tar, st.transition_time / st.transition_max);
}

CE_END_NAMESPACE