#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Animator::_Animator() : CE_COMPDEF(Animator), _graph(nullptr) {}

void _Animator::graph(const AnimGraph& g) {
    _graph = g;

}

void _Animator::OnUpdate() {

}

CE_END_NAMESPACE