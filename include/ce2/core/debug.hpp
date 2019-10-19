#pragma once
#include "chokoengine.hpp"
#include "enums/io_terminal_color.hpp"

CE_BEGIN_NAMESPACE

class Debug { CE_CLASS_COMMON
	static CE_EXPORT std::ofstream logStream;

	static bool Init();
public:
	static void Message(const std::string& caller, const std::string& message, TerminalColor col = TerminalColor::Default);
	static void Warning(const std::string& caller, const std::string& message);
	static void Error(const std::string& caller, const std::string& message);

	static std::vector<uintptr_t> StackTrace(uint count);
	static uint StackTrace(uint count, uintptr_t* frames);
};

CE_END_NAMESPACE
