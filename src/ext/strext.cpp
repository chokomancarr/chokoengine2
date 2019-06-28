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
			o.push_back(s.substr(off, pos - off));
		}
		off = pos + 1;
	} while (pos != std::string::npos);
	return o;
}

std::u32string StrExt::ToUnicode(const std::string& s) {
#define MK(cc) uint(*(cc) & 63)
	char* cc = (char*)s.data();
	std::u32string result = {};
	for (uint a = 0; *cc > 0; a++) {
		if (!((*cc >> 7) & 1)) {
			result.push_back(*cc++);
		}
		else if (!((*cc >> 5) & 1)) {
			uint res = *cc & 31;
			res = (res << 6) + MK(cc + 1);
			cc += 2;
			result.push_back(res);
		}
		else if (!((*cc >> 4) & 1)) {
			uint res = *cc & 15;
			res = (res << 12) + (MK(cc + 1) << 6) + MK(cc + 2);
			cc += 3;
			result.push_back(res);
		}
		else {
			uint res = *cc & 7;
			res = (res << 18) + (MK(cc + 1) << 12) + (MK(cc + 2) << 6) + MK(cc + 3);
			cc += 4;
			result.push_back(res);
		}
	}
	return result;
#undef MK
}

std::string StrExt::FromUnicode(const std::u32string& s) {
	std::string result;
	result.reserve(s.size());
	for (auto& i : s) {
		if (i <= 0x7f) {
			result.push_back(static_cast<char>(i));
		}
		else if (i <= 0x7ff) {
			result.push_back(static_cast<char>(0xc0 | ((i >> 6) & 0x1f)));
			result.push_back(static_cast<char>(0x80 | (i & 0x3f)));
		}
		else if (i <= 0xffff) {
			result.push_back(static_cast<char>(0xe0 | ((i >> 12) & 0x0f)));
			result.push_back(static_cast<char>(0x80 | ((i >> 6) & 0x3f)));
			result.push_back(static_cast<char>(0x80 | (i & 0x3f)));
		}
		else {
			result.push_back(static_cast<char>(0xf0 | ((i >> 18) & 0x07)));
			result.push_back(static_cast<char>(0x80 | ((i >> 12) & 0x3f)));
			result.push_back(static_cast<char>(0x80 | ((i >> 6) & 0x3f)));
			result.push_back(static_cast<char>(0x80 | (i & 0x3f)));
		}
	}
	return result;
}

CE_END_NAMESPACE