/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <memory>

#include "Shader.h"


namespace Shaders {
	inline std::unique_ptr<Shader> posColor = nullptr;
	inline std::unique_ptr<Shader> text = nullptr;
	inline std::unique_ptr<Shader> alphaChecker = nullptr;
	inline std::unique_ptr<Shader> hue = nullptr;
	inline std::unique_ptr<Shader> postProcess = nullptr;

	//MAIN MENU SHADERS
	inline std::unique_ptr<Shader> city = nullptr;
	inline std::unique_ptr<Shader> ghost = nullptr;
	inline std::unique_ptr<Shader> fractal = nullptr;
	inline std::unique_ptr<Shader> happy = nullptr;
	inline std::unique_ptr<Shader> mobiusSphere = nullptr;
	inline std::unique_ptr<Shader> hexagon = nullptr;
	inline std::unique_ptr<Shader> bubble = nullptr;

	void initShaders();
}