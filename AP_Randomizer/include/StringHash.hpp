#pragma once
#include <string>

// wstring hashes to run switch statements on strings.
namespace StringHash {
	// std::hash isn't constexpr so we need to implement our own hash function.
	// I just copy/pasted this from https://stackoverflow.com/a/48896410
	constexpr size_t HashWstring(const std::wstring& to_hash) {
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
}