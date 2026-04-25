/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string>

static const std::string ALPHA_CHECKER_FRAG = R"(#version 330 core

in vec4 vertexColor;

out vec4 FragColor;

uniform float alphaCheckerX;
uniform float alphaCheckerY;
uniform float alphaCheckerSize;

void main()
{
    vec2 coord = vec2(alphaCheckerX, alphaCheckerY) - gl_FragCoord.xy;
    int cellX = int(floor(coord.x / alphaCheckerSize));
    int cellY = int(floor(coord.y / alphaCheckerSize));

    if (mod((cellX + cellY), 2) == 0)
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    else
        FragColor = vec4(0.8, 0.8, 0.8, 1.0);
}
)";