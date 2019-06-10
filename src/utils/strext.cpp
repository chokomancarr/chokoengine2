#include "chokoengine_common.hpp"
#include <locale>
#include <codecvt>

BEGIN_CE_NAMESPACE

std::wstring StrExt::Widen(const std::string& s) {
	return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.from_bytes(s);
}

std::string StrExt::Unwiden(const std::wstring& s) {
	return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.to_bytes(s);
}

END_CE_NAMESPACE