/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <windows.h>
#include <cstdint>

namespace TrampolineUnwind {
void Register(void* pTrampoline, void* pOriginalFunction, uint32_t trampolineSize);
void CleanupAll();
}
