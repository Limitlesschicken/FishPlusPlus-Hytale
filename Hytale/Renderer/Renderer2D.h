/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <memory>

#include "Mesh.h"
#include "../Math/Vector2.h"
#include "../Math/Color.h"

class Renderer2D {
public:
	static inline std::unique_ptr<Renderer2D> colored;
	static inline std::unique_ptr<Renderer2D> alphaChecker;
	static inline std::unique_ptr<Renderer2D> hue;
	std::unique_ptr<Mesh> lines = nullptr;
	std::unique_ptr<Mesh> triangles = nullptr;

	Renderer2D(Shader* shader);

	static void InitRenderer();

	void BeginScissor(float x, float y, float width, float height);
	void EndScissor();

	void Square(Vector2 Position, float width, float height, Color color);
	void SquareMultiColor(Vector2 p1, float width, float height, Color tl, Color tr, Color bl, Color br);
	void SquareOutline(Vector2 p1, float width, float height, Color color, Color outlineColor);

	void Line(Vector2 p1, Vector2 p2, Color color);

	virtual void Render();
private:

	Shader* shader;
};