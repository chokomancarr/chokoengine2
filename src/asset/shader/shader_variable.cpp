#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

ShaderVariable::ShaderVariable(const std::string& s, ShaderVariableType t, GLuint l)
	: _name(s), _type(t), _location(l),
	_val_f(0),
	_val_i(0),
	_val_v2(0),
	_val_v3(0),
	_val_v4(0),
	_val_m(1),
	_val_t(nullptr) {}

CE_END_NAMESPACE