/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

struct App;

struct Input {
	char pad[0x8];
	App* app;
	char pad2[0x80];
	bool isMouseLocked;
};