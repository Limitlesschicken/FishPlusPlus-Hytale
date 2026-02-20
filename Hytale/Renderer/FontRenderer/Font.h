/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include <map>
#include <string>

#include "Character.h"
#include "../../Math/Color.h"


class Font {
public:
	Font(std::map<char, Character> characters, int size);

	void RenderText(std::string text, float x, float y, float scale, Color color);
	void RenderTextShadow(std::string text, float x, float y, float scale, Color color);

	float getWidth(std::string text);
	float getHeight();
private:
	std::map<char, Character> m_characters;

	int m_size;

	uint32_t vao;
	uint32_t vbo;
};