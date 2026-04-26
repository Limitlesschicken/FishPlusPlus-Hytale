/*
 * Copyright (c) FishPlusPlus.
 */
#include "Shaders.h"
#include <string>

#include "shaders/PosColor.h"
#include "shaders/Text.h"
#include "shaders/AlphaChecker.h"
#include "shaders/Hue.h"
#include "shaders/PostProcess.h"
#include "shaders/MainMenu/Bubble.h"
#include "shaders/MainMenu/MobiusSphere.h"
#include "shaders/MainMenu/Hexagon.h"
#include "shaders/MainMenu/City.h"
#include "shaders/MainMenu/Ghost.h"
#include "shaders/MainMenu/Fractal.h"
#include "shaders/MainMenu/Happy.h"
#include "shaders/MainMenu/Cells.h"

void Shaders::initShaders() {
    posColor = std::make_unique<Shader>(POSCOLOR_VERT, POSCOLOR_FRAG);
    text = std::make_unique<Shader>(TEXT_VERT, TEXT_FRAG);
    alphaChecker = std::make_unique<Shader>(POSCOLOR_VERT, ALPHA_CHECKER_FRAG);
    hue = std::make_unique<Shader>(POSCOLOR_VERT, HUE_FRAG);
	postProcess = std::make_unique<Shader>(POSTPROCESS_VERT, POSTPROCESS_FRAG);

	city = std::make_unique<Shader>(POSCOLOR_VERT, CITY_FRAG);
    ghost = std::make_unique<Shader>(POSCOLOR_VERT, GHOST_FRAG);
	fractal = std::make_unique<Shader>(POSCOLOR_VERT, FRACTAL_FRAG);
	happy = std::make_unique<Shader>(POSCOLOR_VERT, HAPPY_FRAG);
	mobiusSphere = std::make_unique<Shader>(POSCOLOR_VERT, MOBIUS_SPHERE_FRAG);
	hexagon = std::make_unique<Shader>(POSCOLOR_VERT, HEXAGON_FRAG);
	bubble = std::make_unique<Shader>(POSCOLOR_VERT, BUBBLE_FRAG);
	cells = std::make_unique<Shader>(POSCOLOR_VERT, CELLS_FRAG);
}