/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#include "Fonts.h"

#include "Fontloader.h"

void Fonts::initFonts() {
	FontLoader fontloader;
	Figtree = std::make_unique<Font>(fontloader.LoadFont(fontloader.Figtree, 24));
}