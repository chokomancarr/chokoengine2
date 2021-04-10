#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

namespace CompCallbackFlag {
	typedef uint32_t Tp;

	const Tp EXEC_IN_EDIT_MODE = 1 << 0;
};

CE_END_NAMESPACE