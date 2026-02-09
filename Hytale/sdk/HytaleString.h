#pragma once

#include <string>
#include <Windows.h>

struct HytaleString {
	char pad[0x8];
	int length;
	char16_t text[64];

	std::string getName() {
		

		int size_needed = WideCharToMultiByte(
			CP_UTF8, 0,
			reinterpret_cast<LPCWCH>(text),
			length,
			nullptr, 0, nullptr, nullptr
		);

		std::string result(size_needed, 0);
		WideCharToMultiByte(
			CP_UTF8, 0,
			reinterpret_cast<LPCWCH>(text),
			length,
			result.data(),
			size_needed, nullptr, nullptr
		);

		return result;
	}

};