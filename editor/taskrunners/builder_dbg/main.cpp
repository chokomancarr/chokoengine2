#include "builder_dbg.hpp"

int main(int argc, char** argv) {
    std::map<std::string, std::vector<std::string>> vars;

    std::string s = "";
    std::vector<std::string> vs = {};

    std::vector<const char*> argnms = {
        "--root", "--configArgs", "--clean"
    };

    for (int a = 0; a < argc; a++) {
        for (auto& g : argnms) {
            if (!strcmp(argv[a], g)) {
                vars[s] = vs;
                s = g;
                vs.clear();
                goto next;
            }
        }
        vs.push_back(argv[a]);
    next:;
    }
    vars[s] = vs;

    BuilderDebug::projectRoot = vars["--root"][0];
    BuilderDebug::cmakeConfigArgs = vars["--configArgs"];
    BuilderDebug::clean = (vars["--clean"][0] == "1");

    return BuilderDebug::Run();
}
