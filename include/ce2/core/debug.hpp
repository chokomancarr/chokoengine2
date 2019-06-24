#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Debug { CE_CLASS_COMMON
	static CE_EXPORT std::ofstream logStream;

	static bool Init();
public:
	static void Message(const std::string& caller, const std::string& message);
	static void Warning(const std::string& caller, const std::string& message);
	static void Error(const std::string& caller, const std::string& message);
};

CE_END_NAMESPACE
