#pragma once
#include <codecvt>
#include <algorithm>
#include "StringOps.hpp"

namespace StringOps {
	using std::string;
	using std::wstring;
	
	string ToNarrow(wstring input) {
		static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(input);
	}

	wstring ToWide(string input) {
		static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(input);
	}
}