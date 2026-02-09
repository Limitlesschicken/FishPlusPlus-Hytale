#pragma once

#include "Window.h"

struct Engine {
	char pad_0000[16]; //0x0000
	Window* Window; //0x0010
};