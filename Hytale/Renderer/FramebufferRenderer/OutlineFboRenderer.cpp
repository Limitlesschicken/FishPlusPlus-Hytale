/*
 * Copyright (c) FishPlusPlus.
 */
#include "OutlineFboRenderer.h"

#include <cmath>

OutlineFboRenderer::OutlineFboRenderer() : FramebufferRenderer(Shaders::postProcess.get()) {}

void OutlineFboRenderer::setupPass(const OutlineUniforms& uniforms) {
    static double start = Util::GetTime();
    double t = Util::GetTime() - start;

    t = std::fmod(t, 1000.0);

    this->shader->bind();
    this->shader->set("screenTexture", 0);
    this->shader->set("u_Size", Vector2((float)winW, (float)winH));
    this->shader->set("u_Time", (float)t);
    this->shader->set("u_Glow", uniforms.glow);
    this->shader->set("u_GlowSize", uniforms.glowSize);
    this->shader->set("u_OutlineColor", uniforms.outlineColor);
	this->shader->set("u_UseEffect", uniforms.wave);
}