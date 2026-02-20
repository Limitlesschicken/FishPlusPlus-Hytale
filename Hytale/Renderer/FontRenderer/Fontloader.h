/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include <map>

#include "Font.h"



class FontLoader {
public:
	enum Fonts {
		Figtree
	};

	FontLoader();

	Font LoadFont(Fonts file, int size);
private:
	FT_Library m_ft;
};


