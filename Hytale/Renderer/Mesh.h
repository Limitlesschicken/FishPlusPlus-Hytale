#pragma once

#include <vector>

#include "Vertex.h"

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	Mesh(bool lines);
	~Mesh();
	void AddVertex3D(const Vertex& vertex);
	void AddVertex2D(const Vertex& vertex);
	void AddTriangle(uint32_t a, uint32_t b, uint32_t c);
	void AddQuad(uint32_t a, uint32_t b, uint32_t c, uint32_t d);
	void AddLine(uint32_t a, uint32_t b);

	void EndMesh();
	void Render();
	void Render2D();
	void AfterRender();

	uint32_t GetVertexCount();

private:
	uint32_t vao;
	uint32_t vbo;
	uint32_t ebo;

	uint32_t m_vertexCount;

	bool lines;
};