#include "builder_dbg.hpp"

using namespace ChokoEngine;

int BuilderDebug::Run(const std::string& projectRoot) {
    Debug::Message("Builder", "Generating debug binary for project \""
        + projectRoot + "\"");

    return 0;
}