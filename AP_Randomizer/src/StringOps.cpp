#pragma once
#include <Windows.h>
#include <codecvt>
#include <algorithm>
#include "StringOps.hpp"
#include "Logger.hpp"

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

	void CopyToClipboard(wstring input) {
		// Shamelessly copied from https://stackoverflow.com/questions/40664890/copy-unicode-string-to-clipboard-isnt-working
		// I have no idea how this works lol.
		const wchar_t* buffer = input.c_str();
		size_t size = sizeof(WCHAR) * (wcslen(buffer) + 1);
		if (!OpenClipboard(0)) {
			Log("Could not open clipboard!", LogType::Warning);
			return;
		}
		HGLOBAL hClipboardData = GlobalAlloc(GMEM_MOVEABLE, size);
		WCHAR* pchData;
		pchData = (WCHAR*)GlobalLock(hClipboardData);
		wcscpy_s(pchData, size / sizeof(wchar_t), buffer);
		GlobalUnlock(hClipboardData);
		SetClipboardData(CF_UNICODETEXT, hClipboardData);
		CloseClipboard();
	}
}