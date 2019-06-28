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
};

CE_END_NAMESPACE