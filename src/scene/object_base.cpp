#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

ChokoEngine::objectid _Object::_idCount = 0;

_Object::_Object(const std::string& nm) : _name(nm), _id(_idCount++) {}

CE_END_NAMESPACE