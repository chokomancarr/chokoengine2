#include "builder_dbg.hpp"

int main(int argc, char** argv) {
    if (argc < 2) return -1;
    return BuilderDebug::Run(argv[1]);
}
