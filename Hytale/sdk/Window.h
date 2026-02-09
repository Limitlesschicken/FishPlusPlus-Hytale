#pragma once

struct Window {
	char pad_0000[9]; //0x0000
	bool IsCursorHidden; //0x0009
	bool IsFocused; //0x000A
	char pad_000B[5]; //0x000B
	void* Handle; //0x0010
	char pad_0018[48]; //0x0018
	uint32_t WindowWidth; //0x0048
	uint32_t WindowHeight; //0x004C
};