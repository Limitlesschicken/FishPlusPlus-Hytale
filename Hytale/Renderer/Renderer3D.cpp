/*
 * Copyright (c) FishPlusPlus.
 */
#include "Renderer3D.h"

#include "core.h"

Renderer3D::Renderer3D() {
	lines = std::make_unique<Mesh>(true);
	triangles = std::make_unique<Mesh>(false);
}

void Renderer3D::Quad(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4, Color color) {
	uint32_t base = triangles->GetVertexCount();
	triangles->AddVertex3D(Vertex(Vector3(p1.x, p1.y, p1.z), color));
	triangles->AddVertex3D(Vertex(Vector3(p2.x, p2.y, p2.z), color));
	triangles->AddVertex3D(Vertex(Vector3(p3.x, p3.y, p3.z), color));
	triangles->AddVertex3D(Vertex(Vector3(p4.x, p4.y, p4.z), color));

	triangles->AddQuad(base + 0, base + 1, base + 2, base + 3);
}

void Renderer3D::Line(Vector3 p1, Vector3 p2, Color color) {
	uint32_t base = lines->GetVertexCount();
	lines->AddVertex3D(Vertex(Vector3(p1.x, p1.y, p1.z), color));
	lines->AddVertex3D(Vertex(Vector3(p2.x, p2.y, p2.z), color));

	lines->AddLine(base + 0, base + 1);
}

void Renderer3D::BoxLines(Vector3 pos, Vector3 size, Color insideColor, Color outlineColor) {
	Quad(pos + Vector3(size.x, 0, 0), pos + Vector3(size.x, 0, size.z), pos + Vector3(size.x, size.y, size.z), pos + Vector3(size.x, size.y, 0), insideColor);
	Quad(pos, pos + Vector3(0, size.y, 0), pos + Vector3(size.x, size.y, 0), pos + Vector3(size.x, 0, 0), insideColor);
	Quad(pos + Vector3(0, size.y, 0), pos + Vector3(0, size.y, size.z), pos + Vector3(size.x, size.y, size.z), pos + Vector3(size.x, size.y, 0), insideColor);
	Quad(pos, pos + Vector3(size.x, 0, 0), pos + Vector3(size.x, 0, size.z), pos + Vector3(0, 0, size.z), insideColor);
	Quad(pos + Vector3(0, 0, size.z), pos + Vector3(size.x, 0, size.z), pos + Vector3(size.x, size.y, size.z), pos + Vector3(0, size.y, size.z), insideColor);
	Quad(pos, pos + Vector3(0, 0, size.z), pos + Vector3(0, size.y, size.z), pos + Vector3(0, size.y, 0), insideColor);


	Line(pos + Vector3(size.x, 0, size.z), pos + Vector3(size.x, size.y, size.z), outlineColor);
	Line(pos + Vector3(0, 0, size.z), pos + Vector3(0, size.y, size.z), outlineColor);
	Line(pos + Vector3(size.x, 0, 0), pos + Vector3(size.x, size.y, 0), outlineColor);
	Line(pos, pos + Vector3(0, size.y, 0), outlineColor);
	Line(pos + Vector3(size.x, size.y, size.z), pos + Vector3(size.x, size.y, 0), outlineColor);
	Line(pos + Vector3(size.x, size.y, size.z), pos + Vector3(0, size.y, size.z), outlineColor);
	Line(pos + Vector3(0, size.y, 0), pos + Vector3(size.x, size.y, 0), outlineColor);
	Line(pos + Vector3(0, size.y, 0), pos + Vector3(0, size.y, size.z), outlineColor);
	Line(pos + Vector3(size.x, 0, size.z), pos + Vector3(size.x, 0, 0), outlineColor);
	Line(pos + Vector3(size.x, 0, size.z), pos + Vector3(0, 0, size.z), outlineColor);
	Line(pos, pos + Vector3(size.x, 0, 0), outlineColor);
	Line(pos, pos + Vector3(0, 0, size.z), outlineColor);
}

void Renderer3D::BoxLines(Entity* entity, Color insideColor, Color outlineColor) {
	ValidPtrVoid(entity);
	if (entity->OldPos == Vector3(0, 0, 0))
		entity->OldPos = entity->Position;

	BoxLines(entity->RenderPos + entity->Hitbox.min, entity->Hitbox.max - entity->Hitbox.min, insideColor, outlineColor);
}

void Renderer3D::Render() {
	triangles->EndMesh();
	triangles->Render();

	lines->EndMesh();
	lines->Render();
}