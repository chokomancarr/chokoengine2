#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Animator::_Animator() : CE_COMPDEF(Animator), _state({}), _single(false),
		_clip(nullptr), _clipTime(0), _graph(nullptr) {
	//_callbackMask &= ~CompCallbackFlag::EXEC_IN_EDIT_MODE;
}

void _Animator::graph(const AnimGraph& g) {
	if (!!(_graph = g)) {
		_state = _graph->GetNewState();
	}
}

void _Animator::clip(const AnimClip& c) {
	if (!!(_clip = c)) {
		_clipTime = _clip->range().x;
	}
}

void _Animator::SetBool(const std::string& name, bool value) {
	if (!_graph) return;
	const auto& vrs = _graph->vars();
	auto it = std::find_if(vrs.begin(), vrs.end(), [&name](const _AnimGraph::Var& v) {
		return v.name == name;
	});
	if (it == vrs.end()) {
		Debug::Warning("Animator::SetBool", "variable \"" + name + "\" does not exist!");
		return;
	}
	if (it->type != _AnimGraph::Var::Type::Bool) {
		Debug::Warning("Animator::SetBool", "variable \"" + name + "\" is not of bool type!");
		return;
	}
	_state.vars[it - vrs.begin()].b = value;
}

void _Animator::SetInt(const std::string& name, int value) {
	if (!_graph) return;
	const auto& vrs = _graph->vars();
	auto it = std::find_if(vrs.begin(), vrs.end(), [&name](const _AnimGraph::Var& v) {
		return v.name == name;
	});
	if (it == vrs.end()) {
		Debug::Warning("Animator::SetInt", "variable \"" + name + "\" does not exist!");
		return;
	}
	if (it->type != _AnimGraph::Var::Type::Int) {
		Debug::Warning("Animator::SetInt", "variable \"" + name + "\" is not of int type!");
		return;
	}
	_state.vars[it - vrs.begin()].i = value;
}

void _Animator::SetFloat(const std::string& name, float value) {
	if (!_graph) return;
	const auto& vrs = _graph->vars();
	auto it = std::find_if(vrs.begin(), vrs.end(), [&name](const _AnimGraph::Var& v) {
		return v.name == name;
	});
	if (it == vrs.end()) {
		Debug::Warning("Animator::SetFloat", "variable \"" + name + "\" does not exist!");
		return;
	}
	if (it->type != _AnimGraph::Var::Type::Float) {
		Debug::Warning("Animator::SetFloat", "variable \"" + name + "\" is not of float type!");
		return;
	}
	_state.vars[it - vrs.begin()].f = value;
}

void _Animator::PlayState(const std::string& name, float time) {
	if (!_graph) return;
	const auto& nds = _graph->nodes();
	auto it = std::find_if(nds.begin(), nds.end(), [&name](const _AnimGraph::Node& v) {
		return v.name == name;
	});
	if (it == nds.end()) {
		Debug::Warning("Animator::PlayState", "state \"" + name + "\" does not exist!");
		return;
	}
	_state.node_current = &*it;
	_state.time_current = time;
	_state.transition_time = -1;
}

_AnimClip::VQ _Animator::Get(const std::string& sig) const {
	if (_single) {
		if (!_clip) return _AnimClip::VQ();
		return _clip->Get(sig, _clipTime);
	}
	else {
		if (!_graph) return _AnimClip::VQ();
		return _graph->Get(_state, sig);
	}
}

void _Animator::OnLateUpdate() {
	if (_single) {
		if (!_clip) return;
		const auto rng = _clip->range();
		if (_clipTime < rng.y)
			_clipTime += Time::delta() * 24;
		else {
			if (_repeat) {
				_clipTime = Math::Repeat<float>(rng.x, rng.y, _clipTime + Time::delta() * 24);
			}
		}
	}
	else {
		if (!_graph) return;
		//
		//_state.vars[0].b = Input::KeyHold(InputKey::W);

		_graph->Update(_state);
	}
}

CE_END_NAMESPACE