#include "chokoengine_common.hpp"

BEGIN_CE_NAMESPACE

std::ofstream Debug::logStream;

bool Debug::Init() {
    return true;
}

void Debug::Message(const std::string& caller, const std::string& msg) {
    std::cout << "[i] " + caller + ": " + msg + "\n";
    std::flush(std::cout);
}

void Debug::Warning(const std::string& caller, const std::string& msg) {
	std::cout << "[w] " + caller + ": " + msg + "\n";
	std::flush(std::cout);
}

void Debug::Error(const std::string& caller, const std::string& msg) {
	std::cerr << "[e] " + caller + ": " + msg + "\n";
	std::flush(std::cout);
}

END_CE_NAMESPACE
