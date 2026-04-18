/*
 * Copyright (c) FishPlusPlus.
 */
#include "OutlineFboRenderer.h"

OutlineFboRenderer::OutlineFboRenderer() : FramebufferRenderer(Shaders::postProcess.get()) {}

void OutlineFboRenderer::setupPass(const OutlineUniforms& uniforms) {
    this->shader->bind();
    this->shader->set("screenTexture", 0);
    this->shader->set("u_Size", Vector2((float)winW, (float)winH));
    this->shader->set("u_Time", (float)Util::GetTime());
    this->shader->set("u_Glow", uniforms.glow);
    this->shader->set("u_GlowSize", uniforms.glowSize);
    this->shader->set("u_OutlineColor", uniforms.outlineColor);
}