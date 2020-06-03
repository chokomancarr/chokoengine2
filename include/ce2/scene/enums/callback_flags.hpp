#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class CompCallbackFlag {
public:
	typedef uint32_t Tp;

	static constexpr Tp EXEC_IN_EDIT_MODE = 1 << 0;
};

CE_END_NAMESPACE