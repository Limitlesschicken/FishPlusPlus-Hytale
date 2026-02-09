#pragma once

#include <memory>

#include "Mesh.h"
#include "../Math/Vector2.h"
#include "../Math/Color.h"

class Renderer2D {
public:
	static inline std::unique_ptr<Renderer2D> colored;
	std::unique_ptr<Mesh> lines = nullptr;
	std::unique_ptr<Mesh> triangles = nullptr;

	Renderer2D();

	void Square(Vector2 Position, float width, float height, Color color);
	void SquareOutline(Vector2 p1, float width, float height, Color color, Color outlineColor);

	void FastLine(Vector2 p1, Vector2 p2, Color color);
	void Line(Vector2 p1, Vector2 p2, float thickness, Color color);

	void Render();
private:
};