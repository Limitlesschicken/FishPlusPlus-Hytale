/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "../Renderer2D.h"
#include "../Shaders.h"
#include "Math/Vector2.h"

class AlphaCheckerRenderer : public Renderer2D {
public:
	AlphaCheckerRenderer(Shader* shader) : Renderer2D(shader) {}

	void SetStartCheckerPos(Vector2 pos) {
		this->startPos = pos;
	}

	void Render() override {
		Shaders::alphaChecker->bind();
		Shaders::alphaChecker->set("alphaCheckerX", startPos.x);
		Shaders::alphaChecker->set("alphaCheckerY", startPos.y);
		Shaders::alphaChecker->set("alphaCheckerSize", 3.0f);

		Renderer2D::Render();
	}

private:
	Vector2 startPos = Vector2(0.0f, 0.0f);
};