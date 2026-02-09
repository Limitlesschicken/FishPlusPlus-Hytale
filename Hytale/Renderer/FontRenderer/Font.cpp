#include "Font.h"

#include <iostream>
#include <map>

#include "glad/glad.h"

#include "../../Math/Vector2.h"
#include "../../Math/Color.h"
#include "Character.h"
#include "../Shaders.h"

Font::Font(std::map<char, Character> characters, int size) {
	m_characters = characters;
	m_size = size;

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
	

	GLint srcFactor, dstFactor;
	glGetIntegerv(GL_BLEND_SRC_ALPHA, &srcFactor);
	glGetIntegerv(GL_BLEND_DST_ALPHA, &dstFactor);

	GLint lastProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
	GLint oldViewport[4];
	glGetIntegerv(GL_VIEWPORT, oldViewport);
	GLboolean blend = glIsEnabled(GL_BLEND);
	GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
	GLboolean cullFace = glIsEnabled(GL_CULL_FACE);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDepthRange(0.0, 1.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int winW = Util::app->Engine->Window->WindowWidth;
	int winH = Util::app->Engine->Window->WindowHeight;

	glViewport(0, 0, winW, winH);

	Shaders::text->bind();
	Shaders::text->set("textColor", color);
	Shaders::text->set("projection", Matrix4x4::Orthographic(0.0f, winW, winH, 0.0f, -1.0f, 1.0f));
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = m_characters[*c];

		float baseline = y + ch.Ascender * scale;

		float xpos = x + ch.Bearing.x * scale;
		float ypos = (baseline - ch.Bearing.y * scale) - 8;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		float vertices[6][4] = {
			{ xpos,     ypos,       0.0f, 0.0f },
			{ xpos,     ypos + h,   0.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 1.0f },

			{ xpos,     ypos,       0.0f, 0.0f },
			{ xpos + w, ypos + h,   1.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.id);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.Advance >> 6) * scale;
	}
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