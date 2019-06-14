#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class StrExt {
public:
	static std::wstring Widen(const std::string& s);
	static std::string Unwiden(const std::wstring& s);
	static std::vector<std::string> Split(const std::string& s, char c, bool rmblank = false);
};

CE_END_NAMESPACE