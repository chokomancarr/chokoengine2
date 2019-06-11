#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

class StrExt {
public:
	static std::wstring Widen(const std::string& s);
	static std::string Unwiden(const std::wstring& s);
	static std::vector<std::string> Split(const std::string& s, char c, bool rmblank = false);
};

END_CE_NAMESPACE