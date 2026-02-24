/*
 * Copyright (c) FishPlusPlus.
 */
#include "FontLoader.h"

#include <iostream>

#include "glad/glad.h"

#include "../../Math/Vector2.h"
#include "Character.h"
#include "Font.h"

#include "../../external/Fonts/figtreemedium.c"


FontLoader::FontLoader() {
	if (FT_Init_FreeType(&m_ft)) {
		std::cout << "Freetype init failed\n";
		return;
	}
}

Font FontLoader::LoadFont(Fonts font, int size)
{
	FT_Face face = nullptr;
	switch (font) {
	case Figtree:
		if (FT_New_Memory_Face(m_ft, FigtreeData, figtreeSize, 0, &face)) {std::cout << "Failed to load font: Figtree\n";}
		break;
	}

	FT_Set_Pixel_Sizes(face, 0, size);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	std::map<char, Character> Characters;

	FT_GlyphSlot g = face->glyph;
	uint32_t w = 0;
	uint32_t h = 0;

	for (unsigned char c = 32; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "Failed to load character: " << c << "\n";
			continue;
		}

		FT_GlyphSlot g = face->glyph;

		w += g->bitmap.width;
		h = std::max(h, g->bitmap.rows);
	}

	uint32_t texture;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int x = 0;

	for (unsigned char c = 32; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			continue;

		FT_GlyphSlot g = face->glyph;


		Character character = {
			Vector2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x,
			face->size->metrics.ascender >> 6,
			(float)x / w,
			(float)g->bitmap.width / (float)w
		};
		Characters.insert(std::pair<char, Character>(c, character));

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		x += g->bitmap.width;

	}

	FT_Done_Face(face);

	return Font(Characters, size, texture, w, h);
}