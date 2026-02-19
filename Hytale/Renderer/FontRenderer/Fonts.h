/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <memory>

#include "Font.h"

namespace Fonts {
	inline std::unique_ptr<Font> Figtree;

	void initFonts();
}