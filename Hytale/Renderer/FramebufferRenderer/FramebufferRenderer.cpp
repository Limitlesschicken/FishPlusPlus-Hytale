/*
 * Copyright (c) FishPlusPlus.
 */
#include "FramebufferRenderer.h"

FramebufferRenderer::FramebufferRenderer(Shader* shader) {
    this->shader = shader;
	this->width = Util::app->Engine->Window->WindowWidth;
	this->height = Util::app->Engine->Window->WindowHeight;

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void FramebufferRenderer::resize(int width, int height) {
    this->width = width;
    this->height = height;

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    if (texture) 
        glDeleteTextures(1, &texture);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FramebufferRenderer::bind() {
    glGetIntegerv(GL_FRAMEBUFFER, &lastFBO);
    glGetFloatv(GL_COLOR_CLEAR_VALUE, lastClearColor);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void FramebufferRenderer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, lastFBO);
	glClearColor(lastClearColor[0], lastClearColor[1], lastClearColor[2], lastClearColor[3]);
}

void FramebufferRenderer::draw() {
    ValidPtrVoid(Util::app);
    ValidPtrVoid(Util::app->Engine);
    ValidPtrVoid(Util::app->Engine->Window);

    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &lastFBO);

    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVAO);
    glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
    glGetIntegerv(GL_VIEWPORT, oldViewport);
    depthTest = glIsEnabled(GL_DEPTH_TEST);
    cullFace = glIsEnabled(GL_CULL_FACE);
    blend = glIsEnabled(GL_BLEND);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthRange(0.0, 1.0);

    winW = Util::app->Engine->Window->WindowWidth;
    winH = Util::app->Engine->Window->WindowHeight;

    glViewport(0, 0, winW, winH);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    this->shader->bind();
    this->shader->set("screenTexture", 0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindFramebuffer(GL_FRAMEBUFFER, lastFBO);
    glUseProgram(lastProgram);

    glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
    if (depthTest) glEnable(GL_DEPTH_TEST);
    if (cullFace)  glEnable(GL_CULL_FACE);
    if (!blend) glDisable(GL_BLEND);
    glBindVertexArray(lastVAO);
}