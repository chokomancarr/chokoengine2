#include "builder_rel.hpp"

int main(int argc, char** argv) {
    std::map<std::string, std::vector<std::string>> vars;

    std::string s = "";
    std::vector<std::string> vs = {};

    std::vector<const char*> argnms = {
        "--root", "--appname"
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

	BuilderRelease::projectRoot = vars["--root"][0];
	BuilderRelease::appName = vars["--appname"][0];

    const auto& projcfg = CE_NS JsonParser::Parse(CE_NS IO::ReadFile(
		BuilderRelease::projectRoot + "config/project.json"
    ));

	const auto& cmk = projcfg.Get("cmake_path");
	if (cmk.type != CE_NS JsonObject::Type::Unknown) {
		BuilderRelease::cmakePath = cmk.string;
	}
	const auto& bargs = projcfg.Get("cmake_build_rel_args").list;
	for (auto& b : bargs) {
		BuilderRelease::cmakeBuildArgs.push_back(b.string);
	}

    return BuilderRelease::Run();
}
