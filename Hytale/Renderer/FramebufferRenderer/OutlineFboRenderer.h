/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "FramebufferRenderer.h"

class OutlineFboRenderer : public FramebufferRenderer {
public:
	struct OutlineUniforms {
		Color outlineColor;
		bool glow;
		int glowSize;
		bool wave;
	};

	OutlineFboRenderer();

	void setupPass(const OutlineUniforms& uniforms);
};