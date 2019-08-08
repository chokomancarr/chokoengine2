#pragma once
#include "ce2/defines.hpp"

/* system headers
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>

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