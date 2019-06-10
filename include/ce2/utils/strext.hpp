#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

class StrExt {
public:
	static std::wstring Widen(const std::string& s);
	static std::string Unwiden(const std::wstring& s);
};

END_CE_NAMESPACE