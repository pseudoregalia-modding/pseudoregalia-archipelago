#pragma once
#include <codecvt>
#include <algorithm>
#include "StringOps.hpp"

namespace StringOps {
	using std::string;
	using std::wstring;
	
	string ToNarrow(wstring input) {
		string output;
		std::transform(input.begin(), input.end(), std::back_inserter(output), [](wchar_t c) {
			return (char)c;
			});
		return output;
	}

	wstring ToWide(string input) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(input);
	}
}