#include "chokoengine.hpp"
#ifndef PLATFORM_WIN
#include <execinfo.h>
#else
#include <DbgHelp.h>
#endif
#include <thread>

CE_BEGIN_NAMESPACE

namespace {
	uint _stacktrace(uint count, void** frames) {
		return
#ifdef PLATFORM_WIN
			CaptureStackBackTrace(0, count, frames, NULL);
#else
			backtrace(frames, count);
#endif
	}

	HANDLE winHandle;

	bool _initstacktrace() {
#ifdef PLATFORM_WIN
		winHandle = GetCurrentProcess();
		return SymInitialize(winHandle, NULL, true);
#endif
	}

	std::vector<std::string> _dumpstacktrace() {
		void* frames[20];
		uint count = _stacktrace(20, frames);
		auto names = std::vector<std::string>(count);
#ifdef PLATFORM_WIN
		auto handle = GetCurrentProcess();
		char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
		PSYMBOL_INFO sym = (PSYMBOL_INFO)buffer;
		sym->SizeOfStruct = sizeof(SYMBOL_INFO);
		sym->MaxNameLen = MAX_SYM_NAME;
		DWORD64 off = 0;
		for (uint a = 0; a < count; a++) {
			DWORD64 frame = (DWORD64)frames[a];
			std::stringstream stream;
			stream << std::hex << frame;
			if (SymFromAddr(handle, frame, &off, sym)) {
				names[a] = std::string((char*)sym->Name)
					+ " (0x" + stream.str() + ")";
			}
			else {
				names[a] = "0x" + stream.str() + " (no pdb loaded)";
			}
	}
#endif
		return names;
}

	void DumpStackTrace() {
		std::cerr << "-------- dumping stack trace --------" << std::endl;
		if (!_initstacktrace()) {
			std::cerr << "cannot load symbols!" << std::endl;
			//return;
		}
		const auto str = _dumpstacktrace();
		for (auto& s : str) {
			std::cerr << s << std::endl;
		}
	}
}

std::ofstream Debug::logStream;

bool Debug::Init() {
    return true;
}

void Debug::Message(const std::string& caller, const std::string& msg, TerminalColor col) {
	std::stringstream ss;
	ss << std::this_thread::get_id();
    std::cout << "[i t=" + std::to_string(Time::actualMillis()) + " thread=" + ss.str() + "] " + IO::ColorOutput(caller + ": " + msg, col) + "\n";
    std::flush(std::cout);
}

void Debug::Warning(const std::string& caller, const std::string& msg) {
	std::stringstream ss;
	ss << std::this_thread::get_id();
	std::cout << IO::ColorOutput("[w t=" + std::to_string(Time::actualMillis()) + " thread=" + ss.str() + "] ", TerminalColor::Yellow) + caller + ": " + msg + "\n";
	std::flush(std::cout);
}

void Debug::Error(const std::string& caller, const std::string& msg) {
	std::stringstream ss;
	ss << std::this_thread::get_id();
	std::cerr << IO::ColorOutput("[e t=" + std::to_string(Time::actualMillis()) + " thread=" + ss.str() + + "] ", TerminalColor::Red) + caller + ": " + msg + "\n";
	std::flush(std::cout);
#ifdef PLATFORM_WIN
	DumpStackTrace();
#endif
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
