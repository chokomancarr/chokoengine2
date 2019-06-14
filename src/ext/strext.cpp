#include "chokoengine.hpp"
#include <locale>
#include <codecvt>

CE_BEGIN_NAMESPACE

std::wstring StrExt::Widen(const std::string& s) {
	return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.from_bytes(s);
}

std::string StrExt::Unwiden(const std::wstring& s) {
	return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.to_bytes(s);
}

std::vector<std::string> StrExt::Split(const std::string& s, char c, bool rm) {
	std::vector<std::string> o = {};
	size_t pos = -1;
	size_t off = 0;
	do {
		pos = s.find_first_of(c, off);
		if (!rm || pos > off) {
			o.push_back(s.substr(off, pos - off + 1));
		}
		off = pos + 1;
	} while (pos != std::string::npos);
	return o;
}

CE_END_NAMESPACE