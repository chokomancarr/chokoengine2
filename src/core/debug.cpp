#include "chokoengine_common.hpp"

BEGIN_CE_NAMESPACE

std::ofstream Debug::logStream;

bool Debug::Init() {
    return true;
}

void Debug::Message(const std::string& caller, const std::string& msg) {
    std::cout << caller + ": " + msg + "\n";
    std::flush(std::cout);
}

END_CE_NAMESPACE
