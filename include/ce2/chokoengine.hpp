#pragma once
#include "defines.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

BEGIN_CE_NAMESPACE

class Engine {
public:
	bool Init();
	void Cleanup();
};

END_CE_NAMESPACE

#include "core/debug.hpp"
#include "core/input.hpp"
#include "core/io.hpp"

#include "utils/strext.hpp"