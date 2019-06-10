#pragma once
#include "defines.hpp"

BEGIN_CE_NAMESPACE
class Engine {
public:
	bool Init();
	void Cleanup();
};

#include "core/debug.hpp"
#include "core/input.hpp"
#include "core/io.hpp"

END_CE_NAMESPACE