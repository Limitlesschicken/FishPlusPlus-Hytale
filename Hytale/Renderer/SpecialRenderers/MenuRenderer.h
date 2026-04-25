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


		if (MainMenuEffect::currentEffect == MainMenuEffects::City) {
			Renderer2D::ChangeShader(Shaders::city.get());
			Shaders::city->bind();
			Shaders::city->set("u_Size", Vector2((float)Util::app->Engine->Window->WindowWidth, (float)Util::app->Engine->Window->WindowHeight));
			Shaders::city->set("u_Time", (float)t);
		}
		else if (MainMenuEffect::currentEffect == MainMenuEffects::Ghost) {
			Renderer2D::ChangeShader(Shaders::ghost.get());
			Shaders::ghost->bind();
			Shaders::ghost->set("u_Size", Vector2((float)Util::app->Engine->Window->WindowWidth, (float)Util::app->Engine->Window->WindowHeight));
			Shaders::ghost->set("u_Time", (float)t);
		}
		else if (MainMenuEffect::currentEffect == MainMenuEffects::Fractal) {
			Renderer2D::ChangeShader(Shaders::fractal.get());
			Shaders::fractal->bind();
			Shaders::fractal->set("u_Size", Vector2((float)Util::app->Engine->Window->WindowWidth, (float)Util::app->Engine->Window->WindowHeight));
			Shaders::fractal->set("u_Time", (float)t);
		}

		else if (MainMenuEffect::currentEffect == MainMenuEffects::Happy) {
			Renderer2D::ChangeShader(Shaders::happy.get());
			Shaders::happy->bind();
			Shaders::happy->set("u_Size", Vector2((float)Util::app->Engine->Window->WindowWidth, (float)Util::app->Engine->Window->WindowHeight));
			Shaders::happy->set("u_Time", (float)t);
		}
		else if (MainMenuEffect::currentEffect == MainMenuEffects::MobiusSphere) {
			Renderer2D::ChangeShader(Shaders::mobiusSphere.get());
			Shaders::mobiusSphere->bind();
			Shaders::mobiusSphere->set("u_Size", Vector2((float)Util::app->Engine->Window->WindowWidth, (float)Util::app->Engine->Window->WindowHeight));
			Shaders::mobiusSphere->set("u_Time", (float)t);
			Shaders::mobiusSphere->set("u_FrameCount", Util::frameCount);
		}
		else if (MainMenuEffect::currentEffect == MainMenuEffects::Hexagon) {
			Renderer2D::ChangeShader(Shaders::hexagon.get());
			Shaders::hexagon->bind();
			Shaders::hexagon->set("u_Size", Vector2((float)Util::app->Engine->Window->WindowWidth, (float)Util::app->Engine->Window->WindowHeight));
			Shaders::hexagon->set("u_Time", (float)t);
		}
		else if (MainMenuEffect::currentEffect == MainMenuEffects::Bubble) {
			Renderer2D::ChangeShader(Shaders::bubble.get());
			Shaders::bubble->bind();
			Shaders::bubble->set("u_Size", Vector2((float)Util::app->Engine->Window->WindowWidth, (float)Util::app->Engine->Window->WindowHeight));
			Shaders::bubble->set("u_Time", (float)t);
		}

		Renderer2D::Render();
	}
};