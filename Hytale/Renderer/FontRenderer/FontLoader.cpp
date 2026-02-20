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

	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "Failed to load character: " << c << "\n";
			continue;
		}

		uint32_t texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			Vector2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x,
			face->size->metrics.ascender >> 6
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(face);

	return Font(Characters, size);
}