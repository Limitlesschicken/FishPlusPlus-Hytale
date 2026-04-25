/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string>



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
