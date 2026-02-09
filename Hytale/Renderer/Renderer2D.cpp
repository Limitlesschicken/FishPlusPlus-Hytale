#include "Renderer2D.h"

#include <cmath>

Renderer2D::Renderer2D() {
	lines = std::make_unique<Mesh>(true);
	triangles = std::make_unique<Mesh>(false);
}

void Renderer2D::Square(Vector2 p1, float width, float height, Color color) {
	uint32_t base = triangles->GetVertexCount();
	triangles->AddVertex2D(Vertex(Vector3(p1.x, p1.y, 0), color));
	triangles->AddVertex2D(Vertex(Vector3(p1.x + width, p1.y, 0), color));
	triangles->AddVertex2D(Vertex(Vector3(p1.x + width, p1.y + height, 0), color));
	triangles->AddVertex2D(Vertex(Vector3(p1.x, p1.y + height, 0), color));

	triangles->AddQuad(base + 0, base + 1, base + 2, base + 3);
}

void Renderer2D::SquareOutline(Vector2 p1, float width, float height, Color color, Color outlineColor) {
	Square(p1, width, height, color);
	FastLine(Vector2(p1.x, p1.y - 1.0f), Vector2(p1.x + width, p1.y - 1.0f), outlineColor);
	FastLine(Vector2(p1.x + 1.0f, p1.y), Vector2(p1.x + 1.0f, p1.y + height), outlineColor);
	FastLine(Vector2(p1.x + width, p1.y), Vector2(p1.x + width, p1.y + height), outlineColor);
	FastLine(Vector2(p1.x, p1.y + height), Vector2(p1.x + width, p1.y + height), outlineColor);
}

void Renderer2D::FastLine(Vector2 p1, Vector2 p2, Color color) {
	uint32_t base = lines->GetVertexCount();
	lines->AddVertex2D(Vertex(Vector3(p1.x, p1.y, 0), color));
	lines->AddVertex2D(Vertex(Vector3(p2.x, p2.y, 0), color));

	lines->AddLine(base + 0, base + 1);
}

void Renderer2D::Line(Vector2 p1, Vector2 p2, float thickness, Color color) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    float len = sqrtf(dx * dx + dy * dy);
    if (len < 0.0001f)
        return;

    float nx = dx / len;
    float ny = dy / len;

    float px = -ny;
    float py = nx;

    float half = thickness * 0.5f;

    float ox = px * half;
    float oy = py * half;

    Vector2 v0 = { p1.x + ox, p1.y + oy };
    Vector2 v1 = { p2.x + ox, p2.y + oy };
    Vector2 v2 = { p2.x - ox, p2.y - oy };
    Vector2 v3 = { p1.x - ox, p1.y - oy };

    if (fabsf(dx) < 0.0001f && nx == 0 && p1.x < p2.x + 0.001f) {
        v0.x += 1.0f;
        v1.x += 1.0f;
        v2.x += 1.0f;
        v3.x += 1.0f;
    }

    uint32_t base = triangles->GetVertexCount();

    triangles->AddVertex2D(Vertex(Vector3(v0.x, v0.y, 0), color));
    triangles->AddVertex2D(Vertex(Vector3(v1.x, v1.y, 0), color));
    triangles->AddVertex2D(Vertex(Vector3(v2.x, v2.y, 0), color));
    triangles->AddVertex2D(Vertex(Vector3(v3.x, v3.y, 0), color));

    triangles->AddQuad(base + 0, base + 1, base + 2, base + 3);
}

void Renderer2D::Render() {
	triangles->EndMesh();
	triangles->Render2D();

	lines->EndMesh();
	lines->Render2D();
}