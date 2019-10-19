#pragma once
#include "chokoeditor.hpp"
#include "script_newtype.hpp"

CE_BEGIN_ED_NAMESPACE

class ScriptVar {
public:
	enum class Type {
		Int,
		Float,
		Double,
		ExtType
	};

	std::string name;
	Type type;

};

CE_END_ED_NAMESPACE