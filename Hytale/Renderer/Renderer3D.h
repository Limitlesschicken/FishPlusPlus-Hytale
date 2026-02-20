/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <memory>

#include "Mesh.h"
#include "../Math/Vector3.h"
#include "../sdk/Entity.h"

class Renderer3D {
public:
	std::unique_ptr<Mesh> lines = nullptr;
	std::unique_ptr<Mesh> triangles = nullptr;

	Renderer3D();

	void Quad(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4, Color color);
	void Line(Vector3 p1, Vector3 p2, Color color);

	void BoxLines(Vector3 pos, Vector3 size, Color insideColor, Color outlineColor);
	void BoxLines(Entity* entity, Color insideColor, Color outlineColor);

	void Render();
private:
};