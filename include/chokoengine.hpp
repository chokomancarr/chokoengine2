#pragma once
#include "ce2/defines.hpp"

#include <algorithm>
#include <array>
#include <assert.h> 
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <vector>

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
