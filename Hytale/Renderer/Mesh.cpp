/*
 * Copyright (c) FishPlusPlus.
 */
#include "Mesh.h"

#include "core.h"

#define NOMINMAX
#include <Windows.h>
#include <algorithm>

#include "glad/glad.h"	
#include "Shaders.h"

Mesh::Mesh(bool lines) {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	m_vertexCount = 0;
	this->lines = lines;
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}
uint32_t Mesh::GetVertexCount() {
	return m_vertexCount;
}

void Mesh::AddVertex3D(const Vertex& vertex) {
	Camera* camera = Util::getCamera();
	ValidPtrVoid(camera);
	Vector3 camPos = camera->Position;
	vertices.push_back(Vertex(Vector3(vertex.position.x - camPos.x, vertex.position.y - camPos.y, vertex.position.z - camPos.z), vertex.color));
	m_vertexCount++;
}

void Mesh::AddVertex2D(const Vertex& vertex) {
	vertices.push_back(vertex);
	m_vertexCount++;
}

void Mesh::AddTriangle(uint32_t a, uint32_t b, uint32_t c) {
	indices.push_back(a);
	indices.push_back(b);
	indices.push_back(c);
}

void Mesh::AddLine(uint32_t a, uint32_t b) {
	indices.push_back(a);
	indices.push_back(b);
}

void Mesh::AddQuad(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
	AddTriangle(a, b, c);
	AddTriangle(a, c, d);
}

void Mesh::EndMesh() {

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Render() {
	if (Util::app->Stage != AppStage::InGame) {
		AfterRender();
		return;
	}

	if (indices.size() <= 0) {
		AfterRender();
		return;
	}

	if (vertices.size() <= 0) {
		AfterRender();
		return;
	}

	SceneRenderer* sceneRenderer = Util::getGameInstance()->SceneRenderer;
	ValidPtrVoid(sceneRenderer);


	GLint lastProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
	GLint oldViewport[4];
	glGetIntegerv(GL_VIEWPORT, oldViewport);
	GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
	GLboolean cullFace = glIsEnabled(GL_CULL_FACE);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthRange(0.0, 1.0);

	Shaders::posColor->bind();
	Shaders::posColor->set("viewMat", sceneRenderer->MPV);
	glBindVertexArray(vao);
	glDrawElements(lines ? GL_LINES : GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glUseProgram(lastProgram);
	if (depthTest) glEnable(GL_DEPTH_TEST);
	if (cullFace)  glEnable(GL_CULL_FACE);

	AfterRender();
}

void Mesh::Render2D(Shader* shader) {
	if (indices.size() <= 0)
		return;

	ValidPtrVoid(Util::app);
	ValidPtrVoid(Util::app->Engine);
	ValidPtrVoid(Util::app->Engine->Window);

	GLint lastVAO;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVAO);
	GLint lastProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
	GLint oldViewport[4];
	glGetIntegerv(GL_VIEWPORT, oldViewport);
	GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
	GLboolean cullFace = glIsEnabled(GL_CULL_FACE);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDepthRange(0.0, 1.0);

	int winW = Util::app->Engine->Window->WindowWidth;
	int winH = Util::app->Engine->Window->WindowHeight;

	glViewport(0, 0, winW, winH);

	shader->bind();
	shader->set("viewMat", Util::orthoProjMat);
	glBindVertexArray(vao);
	glDrawElements(lines ? GL_LINES : GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glUseProgram(lastProgram);

	glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
	if (depthTest) glEnable(GL_DEPTH_TEST);
	if (cullFace)  glEnable(GL_CULL_FACE);
	glBindVertexArray(lastVAO);

	AfterRender();
}

void Mesh::AfterRender() {
	vertices.clear();
	indices.clear();
	m_vertexCount = 0;
}