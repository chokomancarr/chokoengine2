#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _AnimGraph::_Var {
public:
	bool b;
	int i;
	float f;
};

class _AnimGraph::Var {
public:
	enum class Type {
		Bool,
		Int,
		Float
	};
	std::string name;
	Type type;
	_Var defaultValue;
	_Var value;
};

CE_END_NAMESPACE