#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Component::_Component(ComponentType t, const std::string& nm) : Object(nm), type(t) {

}

CE_END_NAMESPACE