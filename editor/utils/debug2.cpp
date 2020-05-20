#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
    void DoTree(std::string& s, const std::vector<SceneObject>& objs, 
            const std::vector<bool>& level, EDebug::obj2strFn fn) {
        auto lvl2 = level;
        lvl2.push_back(true);
        auto sz = objs.size();
        if (!sz) return;
        for (size_t a = 0; a < sz; a++) {
            std::string sl = "";
            for (auto l : level) {
                sl += l ? "|  " : "   ";
            }
            sl += (a == sz - 1) ? "`--" : "+--";
            s += sl + fn(objs[a]) + "\n";
            if (a == sz - 1)
                lvl2.back() = false;
            DoTree(s, objs[a]->children(), lvl2, fn);
        }
    }
}

int EDebug::printLevel = EDEBUG_LVL_VERBOSE;

std::string EDebug::ObjTree(const std::vector<SceneObject>& objs, obj2strFn fn) {
    std::string res;
    DoTree(res, objs, {}, fn);
    return res;
}

void EDebug::Log(const std::string& caller, const std::string& msg, const int level) {
	constexpr TerminalColor colors[] = {
		TerminalColor::LightGrey,
		TerminalColor::White,
		TerminalColor::Yellow
	};
	if (level == EDEBUG_LVL_ERROR) {
		Debug::Error("Editor::" + caller, msg);
	}
	else if (level >= printLevel) {
		Debug::Message("Editor::" + caller, msg, colors[level]);
	}
}

CE_END_ED_NAMESPACE