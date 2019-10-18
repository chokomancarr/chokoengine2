#include "chokoengine.hpp"
#ifndef PLATFORM_WIN
#include <execinfo.h>
#endif

CE_BEGIN_NAMESPACE

std::ofstream Debug::logStream;

bool Debug::Init() {
    return true;
}

void Debug::Message(const std::string& caller, const std::string& msg) {
    std::cout << "[i] " + caller + ": " + msg + "\n";
    std::flush(std::cout);
}

void Debug::Warning(const std::string& caller, const std::string& msg) {
	std::cout << "\033[33m[w]\033[0m " + caller + ": " + msg + "\n";
	std::flush(std::cout);
}

void Debug::Error(const std::string& caller, const std::string& msg) {
	std::cerr << "\033[31m[e]\033[0m " + caller + ": " + msg + "\n";
	std::flush(std::cout);
}

std::vector<uintptr_t> Debug::StackTrace(uint count) {
	std::vector<uintptr_t> result(count, 0);
#ifdef PLATFORM_WIN
	CaptureStackBackTrace(0, count, (void**)result.data(), NULL);
#else
	backtrace((void**)result.data(), count);
#endif
	return result;
}

uint Debug::StackTrace(uint count, uintptr_t* result) {
#ifdef PLATFORM_WIN
	return CaptureStackBackTrace(0, count, (void**)result, NULL);
#else
	return backtrace((void**)result, count);
#endif
}

CE_END_NAMESPACE
