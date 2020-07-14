#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Animator::_Animator() : CE_COMPDEF(Animator), _state({}), _single(false),
		_clip(nullptr), _clipTime(0), _graph(nullptr) {
	//_callbackMask &= ~CompCallbackFlag::EXEC_IN_EDIT_MODE;
}

void _Animator::graph(const AnimGraph& g) {
    _graph = g;
	_state = _graph->GetNewState();
}

void _Animator::clip(const AnimClip& c) {
	if (!!(_clip = c)) {
		_clipTime = _clip->range().x;
	}
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
		_state.vars[0].b = Input::KeyHold(InputKey::W);

		_graph->Update(_state);
	}
}

CE_END_NAMESPACE