/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string>

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