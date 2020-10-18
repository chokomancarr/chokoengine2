#pragma once
#include "ce2/defines.hpp"
#include <cstdint>

CE_BEGIN_NAMESPACE

class ChokoLait;

class ChokoEngine {
public:
	typedef uint64_t objectid;

	/* is the program running in editor mode?
	 * you should NEVER change this value
	 */
	static CE_EXPORT bool is_editor;

	static bool Init();
	static void Cleanup();
};

CE_END_NAMESPACE

#include "ce2/chokoengine_includes.hpp"
