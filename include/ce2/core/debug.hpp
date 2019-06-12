#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

class Debug { CE_CLASS_COMMON
	static std::ofstream logStream;

	static bool Init();
public:
	static void Message(const std::string& caller, const std::string& message);
	static void Warning(const std::string& caller, const std::string& message);
	static void Error(const std::string& caller, const std::string& message);
};

END_CE_NAMESPACE
