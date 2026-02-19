/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "GLAD/glad.h"

#include <iostream>
#include <memory>
#include <Windows.h>
#include <mutex>

#include "sdk/App.h"
#include "sdk/DefaultMovementController.h"
#include "FeatureDispatcher/FeatureDispatcher.h"
#include "Memory/Memory.h"
#include "external/minhook/minhook.h"
#include "Util/InputSystem.h"

#include "Menu/Menu.h"

#include "Util/Util.h"

#include "sdk/SDK.h"

#include "Renderer/Mesh.h"
#include "Renderer/Shaders.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Renderer3D.h"
#include "Renderer/FontRenderer/Fonts.h"

#include "Math/Matrix4x4.h"

#define ValidPtr(ptr) if (!Util::IsValidPtr(ptr)) return nullptr;
#define ValidPtrVoid(ptr) if (!Util::IsValidPtr(ptr)) return;
#define ValidPtrBool(ptr) if (!Util::IsValidPtr(ptr)) return false;
#define ValidPtrEmpty(ptr) if (!Util::IsValidPtr(ptr)) return {};
#define ValidPtrLoop(ptr) if (!Util::IsValidPtr(ptr)) continue;

namespace Core {
	inline HMODULE MODULEPTR = nullptr;
}