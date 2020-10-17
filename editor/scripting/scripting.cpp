#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
	void rmlinecomment(std::string& s) {
		auto res = std::strstr(s.data(), "//");
		if (res) {
			s.resize(res - s.data());
		}
	}
	int bracketCnt(const std::string& s) {
		int i = 0;
		for (auto& c : s) {
			if (c == '{') i++;
			else if (c == '}') i--;
		}
		return i;
	}

	std::string wordInParen(const std::string& s, size_t st) {
		auto b1 = s.find('(', st);
		auto b2 = s.find(')', b1);
		return s.substr(b1 + 1, b2 - b1 - 1);
	}

	#define SPL (c == ' ' || c == '\t' || c == '\r'\
		|| c == '\n' || c == ',' || c == ';')
	std::string nextWord(const std::string& s, size_t st, size_t* nxt = nullptr) {
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

	bool parseVar(const std::string& ln, size_t& st, ScriptVar& vr, const ScriptInfo& res) {
		auto& tp = vr.name = nextWord(ln, st, &st);
		if (tp.substr(0, 12) == "std::vector<" && tp.back() == '>') {
			vr.is_vector = true;
			tp = tp.substr(12);
			tp.pop_back();
		}
		else {
			vr.is_vector = false;
		}
		static const std::string typeSs[] = {
			"int", "float",
			"Vec2", "Vec3", "Vec4"
		};
		for (int i = 0; i < (sizeof(typeSs) / sizeof(std::string)); i++) {
			if (tp == typeSs[i]) {
				vr.type = (ScriptVar::Type)i;
				return true;
			}
		}
		
		for (auto& a : res->classes) {
			if (tp == a.name) {
				vr.type = ScriptVar::Type::Class;
				vr.sub_class = tp;
				return true;
			}
		}

		for (auto& a : AssetTypeStr) {
			if (tp == a.second) {
				vr.type = ScriptVar::Type::Asset;
				vr.type_asset = a.first;
				vr.sub_class = tp;
				return true;
			}
		}
		if (tp == "SceneObject") {
			vr.type = ScriptVar::Type::SceneObject;
			return true;
		}
		for (int a = 0; a < (int)ComponentType::_COUNT; a++) {
			if (tp == ComponentTypeStr.at((ComponentType)a)) {
				vr.type = ScriptVar::Type::Component;
				vr.type_comp = (ComponentType)a;
				return true;
			}
		}
		/*
		for (auto& t : res->types) {
			if (t->name == tp) {
				vr.type = ScriptVar::Type::ExtType;
				vr.typeExt = t;
				return true;
			}
		}
		*/
		return false;
	}
}

void EScripting::Init() {
	_DummyScriptLoader::Init();
}

#define LN (" at line " + std::to_string(line))

#define ERRT(msg) Debug::Error("ScriptParser", msg);\
	return nullptr

ScriptInfo EScripting::ParseInfo(const std::string& sig) {
	ScriptInfo res = ScriptInfo::New();
	res->sig = sig;
	std::ifstream strm(CE_DIR_ASSET + sig);
	res->name(StrExt::RemoveExt(StrExt::RemoveFd(sig)));
	std::string ln;
	int br = 0;
	int line = 1;
	bool isclass = false;
	bool issubclass = false;
	ScriptClass subclass;
	int cbr, sbr;
	while (std::getline(strm, ln)) {
		rmlinecomment(ln);
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

			if (!issubclass) {
				auto st = (size_t)std::strstr(ln.c_str(), "CE_SERIALIZABLE");
				if (st > 0) {
					st = st - (size_t)ln.c_str() + 16;
					std::string nm = nextWord(ln, st, &st);
					if (nm != "class") {
						ERRT("Expected 'class' after CE_SERIALIZABLE!");
					}
					subclass.name = nextWord(ln, st, &st);
					subclass.vars.clear();
					issubclass = true;
					Debug::Message("Script Var", "subclass \"" + subclass.name + "\" start" + LN);
				}
			}
			else if (br == cbr) {
				issubclass = false;
				res->classes.push_back(subclass);
				Debug::Message("Script Var", "subclass \"" + subclass.name + "\" end" + LN);
			}


			auto st = (size_t)std::strstr(ln.c_str(), "CE_SERIALIZE");
			if (st > 0) {
				ScriptVar vr;
				st = st - (size_t)ln.c_str() + 13;
				if (!parseVar(ln, st, vr, res))
					continue;

				std::string nm = nextWord(ln, st, &st);
				do {
					Debug::Message("Script Var", "var \"" + nm + "\" (" + vr.name + (vr.is_vector ? " array)" : ")") + LN);
					vr.name = nm;
					if (issubclass) {
						subclass.vars.push_back(vr);
					}
					else {
						res->vars.push_back(vr);
					}
					nm = nextWord(ln, st, &st);
				} while (nm.length() > 0);
			}
		}
		line++;
	}
	if (br > 0) {
		ERRT("Expected '}' at EOF!");
	}
	return nullptr;
}

CE_END_ED_NAMESPACE