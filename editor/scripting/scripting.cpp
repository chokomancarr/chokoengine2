#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

inline int bracketCnt(const std::string& s) {
	int i = 0;
	for (auto& c : s) {
		if (c == '{') i++;
		else if (c == '}') i--;
	}
	return i;
}

inline std::string wordInParen(const std::string& s, size_t st) {
	auto b1 = s.find('(', st);
	auto b2 = s.find(')', b1);
	return s.substr(b1 + 1, b2 - b1 - 1);
}

#define SPL (c == ' ' || c == '\t' || c == '\r'\
	|| c == '\n' || c == ',' || c == ';')
inline std::string nextWord(const std::string& s, size_t st, size_t* nxt = nullptr) {
	auto nsp = std::find_if(s.begin() + st, s.end(), [](const char c) {
		return !SPL;
	});
	if (nsp == s.end()) return "";
	auto sp = std::find_if(nsp, s.end(), [](const char c) {
		return SPL;
	});
	if (nxt) *nxt = sp - s.begin();
	return s.substr(nsp - s.begin(), sp - nsp);
}

#define LN (" at line " + std::to_string(line))

#define ERRT(msg) Debug::Error("ScriptParser", msg);\
	return ScriptInfo()

ScriptInfo Scripting::ParseInfo(const std::string& sig) {
	ScriptInfo res;
	std::ifstream strm(sig);
	res.name = StrExt::RemoveExt(StrExt::RemoveFd(sig));
	std::string ln;
	int br = 0;
	int line = 1;
	bool isclass = false;
	while (std::getline(strm, ln)) {
		br += bracketCnt(ln);
		if (br < 0) {
			ERRT("Unexpected '}'" + LN
				+ "! (Parser does not support macro'd brackets!)");
		}
		if (!isclass) {
			auto ps = StrExt::Find(ln, "CE_SCRIPT_DEF");
			if (ps != std::string::npos) {
				//if (br > 0) {
				//	ERRT("Script class" + LN + " must be in global scope!");
				//}
				isclass = true;
				res.className = wordInParen(ln, ps + 13);
			}
		}
		else {
			if (br == 0) {
				return res;
			}
			auto st = StrExt::Find(ln, "CE_SERIALIZE");
			if (st != std::string::npos) {
				auto tp = nextWord(ln, st + 13, &st);
				std::string nm = nextWord(ln, st, &st);
				do {
					Debug::Message("Script Var", "\"" + tp + "\" = \"" + nm + "\" " + LN);
					ScriptVar vr;
					vr.type = ScriptVar::Type::Int;
					vr.name = nm;
					res.vars.push_back(vr);
					nm = nextWord(ln, st, &st);
				} while (nm.length() > 0);
			}
		}
		line++;
	}
	ERRT("Expected '}' at EOF!");
}

CE_END_ED_NAMESPACE