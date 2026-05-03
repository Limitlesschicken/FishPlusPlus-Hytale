/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "../Renderer2D.h"
#include "../Shaders.h"
#include "Math/Vector2.h"

#include <Features/ActualFeatures/MainMenuEffect.h>

class MenuRenderer : public Renderer2D {
public:
	MenuRenderer(Shader* shader) : Renderer2D(shader) {}

	void Render() override {
		static double start = Util::GetTime();
		double t = Util::GetTime() - start;

		t = std::fmod(t, 1000.0);

		Shader* shader = nullptr;
		switch (MainMenuEffect::currentEffect) {
		case MainMenuEffects::City:
			shader = Shaders::city.get();
			break;

		case MainMenuEffects::Ghost:
			shader = Shaders::ghost.get();
			break;

		case MainMenuEffects::Fractal:
			shader = Shaders::fractal.get();
			break;

		case MainMenuEffects::Happy:
			shader = Shaders::happy.get();
			break;

		case MainMenuEffects::MobiusSphere:
			shader = Shaders::mobiusSphere.get();
			break;

		case MainMenuEffects::Hexagon:
			shader = Shaders::hexagon.get();
			break;

		case MainMenuEffects::Bubble:
			shader = Shaders::bubble.get();
			break;

		case MainMenuEffects::Cells:
			shader = Shaders::cells.get();
			break;
		case MainMenuEffects::Octagram:
			shader = Shaders::octagram.get();
			break;
		}

		if (shader) {
			Renderer2D::ChangeShader(shader);

			shader->bind();

			shader->set("u_Size", Vector2((float)Util::app->Engine->Window->WindowWidth, (float)Util::app->Engine->Window->WindowHeight));
			shader->set("u_Time", (float)t);
			shader->set("u_FrameCount", Util::frameCount);
		}



		Renderer2D::Render();
	}
};