#pragma once
#include "ce2/defines.hpp"
#include <cstdint>

CE_BEGIN_NAMESPACE

class ChokoLait;

class ChokoEngine {
public:
	typedef uint64_t objectid;

	static bool Init();
	static void Cleanup();
};

CE_END_NAMESPACE

#include "ce2/chokoengine_includes.hpp"
