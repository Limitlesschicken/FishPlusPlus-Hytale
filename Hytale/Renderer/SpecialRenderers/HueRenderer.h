#pragma once

#include "core.h"
#include "Math/Vector2.h"

class HueRenderer : public Renderer2D {
public:
	HueRenderer(Shader* shader) : Renderer2D(shader) {}

	void SetupUniformInfo(float yPos, float height) {
		this->yPos = yPos;
		this->height = height;
	}

	void Render() override {
		App* app = Util::app;
		ValidPtrVoid(app);
		Engine* engine = app->Engine;
		ValidPtrVoid(engine);
		Window* window = engine->Window;
		ValidPtrVoid(window);
		
		Shaders::hue->bind();
		Shaders::hue->set("yPos", window->WindowHeight - (this->yPos + this->height));
		Shaders::hue->set("height", this->height);

		Renderer2D::Render();
	}

private:
	float yPos = 0;
	float height = 0;
};