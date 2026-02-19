/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#include "Renderer2D.h"

#include "SpecialRenderers/AlphaCheckerRenderer.h"
#include "SpecialRenderers/HueRenderer.h"

#include "core.h"

#include "glad/glad.h"
#include "Util/Util.h"

Renderer2D::Renderer2D(Shader* shader) {
	this->shader = shader;
	lines = std::make_unique<Mesh>(true);
	triangles = std::make_unique<Mesh>(false);
}

void Renderer2D::InitRenderer() {
	Renderer2D::colored = std::make_unique<Renderer2D>(Shaders::posColor.get());
	Renderer2D::alphaChecker = std::make_unique<AlphaCheckerRenderer>(Shaders::alphaChecker.get());
	Renderer2D::hue = std::make_unique<HueRenderer>(Shaders::hue.get());
}

void Renderer2D::BeginScissor(float x, float y, float width, float height) {

	ValidPtrVoid(Util::app);
	ValidPtrVoid(Util::app->Engine);
	ValidPtrVoid(Util::app->Engine->Window);

    glEnable(GL_SCISSOR_TEST);
    glScissor((GLint)x, (GLint)(Util::app->Engine->Window->WindowHeight - (y + height)), (GLint)width, (GLint)height);
}
void Renderer2D::EndScissor() {
    glDisable(GL_SCISSOR_TEST);
}

void Renderer2D::Square(Vector2 p1, float width, float height, Color color) {
	uint32_t base = triangles->GetVertexCount();
	triangles->AddVertex2D(Vertex(Vector3(p1.x, p1.y, 0), color));
	triangles->AddVertex2D(Vertex(Vector3(p1.x + width, p1.y, 0), color));
	triangles->AddVertex2D(Vertex(Vector3(p1.x + width, p1.y + height, 0), color));
	triangles->AddVertex2D(Vertex(Vector3(p1.x, p1.y + height, 0), color));

	triangles->AddQuad(base + 0, base + 1, base + 2, base + 3);
}

void Renderer2D::SquareMultiColor(Vector2 p1, float width, float height, Color tl, Color tr, Color bl, Color br) {
	uint32_t base = triangles->GetVertexCount();
	triangles->AddVertex2D(Vertex(Vector3(p1.x, p1.y, 0), tl));
	triangles->AddVertex2D(Vertex(Vector3(p1.x + width, p1.y, 0), tr));
	triangles->AddVertex2D(Vertex(Vector3(p1.x + width, p1.y + height, 0), br));
	triangles->AddVertex2D(Vertex(Vector3(p1.x, p1.y + height, 0), bl));

	triangles->AddQuad(base + 0, base + 1, base + 2, base + 3);
}


void Renderer2D::SquareOutline(Vector2 p1, float width, float height, Color color, Color outlineColor) {
	Square(p1, width, height, color);
	Line(Vector2(p1.x, p1.y - 1.0f), Vector2(p1.x + width, p1.y - 1.0f), outlineColor);
	Line(Vector2(p1.x + 1.0f, p1.y), Vector2(p1.x + 1.0f, p1.y + height), outlineColor);
	Line(Vector2(p1.x + width, p1.y), Vector2(p1.x + width, p1.y + height), outlineColor);
	Line(Vector2(p1.x, p1.y + height), Vector2(p1.x + width, p1.y + height), outlineColor);
}

void Renderer2D::Line(Vector2 p1, Vector2 p2, Color color) {
	uint32_t base = lines->GetVertexCount();
	lines->AddVertex2D(Vertex(Vector3(p1.x, p1.y, 0), color));
	lines->AddVertex2D(Vertex(Vector3(p2.x, p2.y, 0), color));

	lines->AddLine(base + 0, base + 1);
}

void Renderer2D::Render() {
	triangles->EndMesh();
	triangles->Render2D(this->shader);

	lines->EndMesh();
	lines->Render2D(this->shader);
}