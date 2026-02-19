/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#include "Shaders.h"
#include <string>

static const std::string POSCOLOR_VERT = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 vertexColor;

uniform mat4 viewMat;

void main() {
    gl_Position = viewMat * vec4(aPos, 1.0);
    vertexColor = aColor;
}
)";

static const std::string POSCOLOR_FRAG = R"(#version 330 core

out vec4 FragColor;
in vec4 vertexColor;

void main() {
    FragColor = vertexColor;
}
)";

static const std::string TEXT_VERT = R"(#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

static const std::string TEXT_FRAG = R"(#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0,
                        texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}
)";

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

static const std::string HUE_FRAG = R"(#version 330 core

in vec4 vertexColor;

out vec4 FragColor;

uniform float yPos;
uniform float height;

vec3 hsv_to_rgb(vec3 hsv) {
    float h = hsv.x, s = hsv.y, v = hsv.z;
    float c = v * s;
    float x = c * (1.0 - abs(mod(h / 60.0, 2.0) - 1.0));
    float m = v - c;
    vec3 rgb;

    if (h < 60.0)      rgb = vec3(c, x, 0);
    else if (h < 120.0) rgb = vec3(x, c, 0);
    else if (h < 180.0) rgb = vec3(0, c, x);
    else if (h < 240.0) rgb = vec3(0, x, c);
    else if (h < 300.0) rgb = vec3(x, 0, c);
    else rgb = vec3(c, 0, x);

    return rgb + vec3(m);
}

void main()
{
    float uv = (gl_FragCoord.y - yPos) / height;
    uv = clamp(uv, 0.0, 1.0);


    float hue = 360 - uv * 360.0;
    vec3 rgb = hsv_to_rgb(vec3(hue, 1.0, 1.0));
    FragColor = vec4(rgb, 1.0);
}
)";



void Shaders::initShaders() {
    posColor = std::make_unique<Shader>(POSCOLOR_VERT, POSCOLOR_FRAG);
    text = std::make_unique<Shader>(TEXT_VERT, TEXT_FRAG);
    alphaChecker = std::make_unique<Shader>(POSCOLOR_VERT, ALPHA_CHECKER_FRAG);
    hue = std::make_unique<Shader>(POSCOLOR_VERT, HUE_FRAG);
}