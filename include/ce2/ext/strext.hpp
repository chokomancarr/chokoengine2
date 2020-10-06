#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class StrExt {
public:
	static std::wstring Widen(const std::string& s);
	static std::string Unwiden(const std::wstring& s);
	static std::vector<std::string> Split(const std::string& s, char c, bool rmblank = false);
	static std::u32string ToUnicode(const std::string& s);
	static std::string FromUnicode(const std::u32string& s);
	static std::string ExtensionOf(const std::string& s);
	static std::string RemoveExt(const std::string& s);
	static std::string FolderOf(const std::string& s);
	static std::string RemoveFd(const std::string& s);
	static std::string ParentFd(const std::string& s);
	static std::string ReplaceAll(std::string s, const std::initializer_list<std::pair<std::string, std::string>>& repl);

	static int ToInt(const std::string& s, int def = 0);
	static float ToFloat(const std::string& s, float def = 0);
};

CE_END_NAMESPACE