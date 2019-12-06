#include "chokoeditor.hpp"
#include "strs_asset.hpp"
#include "strs_comp.hpp"

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

inline bool parseVar(const std::string& ln, size_t& st, ScriptVar& vr, const ScriptInfo& res) {
	const auto& tp = vr.typeName = nextWord(ln, st, &st);
	static const std::string typeSs[] = {
		"bool", "int", "float", "double"
		, "Vec2", "Vec3", "Vec4"
	};
	for (int i = 0; i < (sizeof(typeSs) / sizeof(std::string)); i++) {
		if (tp == typeSs[i]) {
			vr.type = (ScriptVar::Type)i;
			return true;
		}
	}
	for (int a = 0; a < (int)AssetType::_COUNT; a++) {
		if (tp == EAssetTypeStrs[a]) {
			vr.type = ScriptVar::Type::Asset;
			vr.typeAsset = (AssetType)a;
			return true;
		}
	}
	for (int a = 0; a < (int)ComponentType::_COUNT; a++) {
		if (tp == ComponentTypeStrs[a]) {
			vr.type = ScriptVar::Type::Comp;
			vr.typeComp = (ComponentType)a;
			return true;
		}
	}
	for (auto& t : res->types) {
		if (t->name == tp) {
			vr.type = ScriptVar::Type::ExtType;
			vr.typeExt = t;
			return true;
		}
	}
	return false;
}

#define LN (" at line " + std::to_string(line))

#define ERRT(msg) Debug::Error("ScriptParser", msg);\
	return nullptr

ScriptInfo Scripting::ParseInfo(const std::string& sig) {
	ScriptInfo res = ScriptInfo::New();
	std::ifstream strm(sig);
	res->name(StrExt::RemoveExt(StrExt::RemoveFd(sig)));
	std::string ln;
	int br = 0;
	int line = 1;
	bool isclass = false;
	int cbr, sbr;
	while (std::getline(strm, ln)) {
		br += bracketCnt(ln);
		if (br < 0) {
			ERRT("Unexpected '}'" + LN
				+ "! (Parser does not support macro'd brackets!)");
		}
		if (!isclass) {
			auto ps = std::strstr(ln.c_str(), "CE_SCRIPT_DEF");
			if (ps) {
				//if (br > 0) {
				//	ERRT("Script class" + LN + " must be in global scope!");
				//}
				isclass = true;
				res->className = wordInParen(ln, ps - ln.c_str() + 13);
				cbr = br;
			}
		}
		else {
			if (br < cbr) {
				return res;
			}
			auto st = (size_t)std::strstr(ln.c_str(), "CE_SERIALIZE");
			if (st) {
				ScriptVar vr;
				st = st - (size_t)ln.c_str() + 13;
				if (!parseVar(ln, st, vr, res))
					continue;

				std::string nm = nextWord(ln, st, &st);
				do {
					Debug::Message("Script Var", "\"" + vr.typeName + "\" = \"" + nm + "\"" + LN);
					vr.name = nm;
					res->vars.push_back(vr);
					nm = nextWord(ln, st, &st);
				} while (nm.length() > 0);
			}
		}
		line++;
	}
	ERRT("Expected '}' at EOF!");
}

CE_END_ED_NAMESPACE