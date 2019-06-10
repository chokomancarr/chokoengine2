#pragma once
#include "defines.hpp"

#include <iostream>
#include <fstream>
#include <string>

BEGIN_CE_NAMESPACE

class Debug {
	static std::ofstream logStream;

public:
	static bool Init();
	static void Message(const std::string& caller, const std::string& message);
};

END_CE_NAMESPACE
