#pragma once
#include <string>

namespace StringOps {
	// Converts wide strings to narrow.
	// No character conversion is used, so non-ascii input will break.
	std::string ToNarrow(std::wstring);

	// Converts UTF-8 inputs to UTF-16 wide strings using std::codecvt.
	std::wstring ToWide(std::string);
	
	// Hashes std::wstrings for executing switch statements on them.
	constexpr size_t HashWstring(const std::wstring& to_hash) {
		// std::hash isn't constexpr so we need to implement our own hash function.
		// I just copy/pasted these from https://stackoverflow.com/a/48896410

		// Require size_t to be 64-bit.
		static_assert(sizeof(size_t) == 8);

		// FNV-1a 64 bit algorithm
		size_t result = 0xcbf29ce484222325; // FNV offset basis
		for (wchar_t c : to_hash) {
			result ^= c;
			result *= 1099511628211; // FNV prime
		}
		return result;
	}

	// Hashes std::strings for executing switch statements on them.
	constexpr size_t HashNstring(const std::string& to_hash) {
		// Require size_t to be 64-bit.
		static_assert(sizeof(size_t) == 8);

		// FNV-1a 64 bit algorithm
		size_t result = 0xcbf29ce484222325; // FNV offset basis
		for (char c : to_hash) {
			result ^= c;
			result *= 1099511628211; // FNV prime
		}
		return result;
	}
}