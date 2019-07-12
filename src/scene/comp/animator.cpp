#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Animator::_Animator() : CE_COMPDEF(Animator), _graph(nullptr) {}

void _Animator::graph(const AnimGraph& g) {
    _graph = g;

}



CE_END_NAMESPACE