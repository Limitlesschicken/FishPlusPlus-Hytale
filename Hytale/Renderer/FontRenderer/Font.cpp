/*
 * Copyright (c) FishPlusPlus.
 */
#include "Font.h"

#include <iostream>
#include <map>

#include "glad/glad.h"

#include "../../Math/Vector2.h"
#include "../../Math/Color.h"
#include "Character.h"
#include "../Shaders.h"

Font::Font(std::map<char, Character> characters, int size, uint32_t textureID, int atlasWidth, int atlasHeight) {
	m_characters = characters;
	m_size = size;
	this->texture = textureID;
	this->atlasWidth = atlasWidth;
	this->atlasHeight = atlasHeight;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	
}

void Font::RenderText(std::string text, float x, float y, float scale, Color color) {
	
	/*
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDepthRange(0.0, 1.0);
	*/

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	Shaders::text->bind();
	Shaders::text->set("textColor", color);
	Shaders::text->set("projection", Util::orthoProjMat);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	std::vector<float> vertices;
	vertices.reserve(text.size() * 6 * 4);

	
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = m_characters[*c];

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		float baseline = y + ch.Ascender * scale;

		float xpos = x + ch.Bearing.x * scale;
		float ypos = (baseline - ch.Bearing.y * scale) - 8;

		x += (ch.Advance >> 6) * scale;

		vertices.insert(vertices.end(), {
			xpos,     ypos,     ch.tx, 0.0f,
			xpos,     ypos + h, ch.tx, ch.Size.y / atlasHeight,
			xpos + w, ypos + h, ch.tx + ch.Size.x / atlasWidth, ch.Size.y / atlasHeight,

			xpos,     ypos,     ch.tx, 0.0f,
			xpos + w, ypos + h, ch.tx + ch.Size.x / atlasWidth, ch.Size.y / atlasHeight,
			xpos + w, ypos,     ch.tx + ch.Size.x / atlasWidth, 0.0f
			});
	}

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Font::RenderTextShadow(std::string text, float x, float y, float scale, Color color) {
	RenderText(text, x + 1, y, scale, Color::Black());
	RenderText(text, x - 1, y, scale, Color::Black());
	RenderText(text, x, y + 1, scale, Color::Black());
	RenderText(text, x, y - 1, scale, Color::Black());
	
	RenderText(text, x, y, scale, color);
}

float Font::getWidth(std::string text) {
	float width = 0.0f;

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = m_characters[*c];

		width += ch.Advance >> 6;
	}
	return width;
}
float Font::getHeight() {
	return m_size;
}