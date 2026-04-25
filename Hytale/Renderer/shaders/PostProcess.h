/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string>

static const std::string POSTPROCESS_VERT = R"(#version 330 core
layout (location = 0) in vec4 vertex;

uniform vec2 u_Size;

out vec2 v_TexCoord;
out vec2 v_OneTexel;

void main()
{
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
    v_TexCoord = vertex.zw;
    v_OneTexel = 1.0 / u_Size;
}
)";

static const std::string POSTPROCESS_FRAG = R"(#version 330 core
in vec2 v_TexCoord;
in vec2 v_OneTexel;

out vec4 FragColor;
uniform sampler2D u_Texture;
uniform vec2 u_Size;
uniform float u_Time;
uniform vec3 u_OutlineColor;
uniform bool u_Glow;
uniform int u_GlowSize;

uniform bool u_UseEffect;

vec2 directionalWaveNormal(vec2 p, float amp, vec2 dir, float freq, float speed, float time, float k) {
    float a = dot(p, dir) * freq + time * speed;
    float b = 0.5 * k * freq * amp * pow((sin(a) + 1.0) * 0.5, k - 1.0) * cos(a);
    return vec2(dir.x * b, dir.y * b);
}

vec3 summedWaveNormal(vec2 p) {

    vec2 sum = vec2(0.0);
    sum += directionalWaveNormal(p, 0.5, normalize(vec2(1, 1)), 5.0, 1.5, u_Time, 1.0);
    sum += directionalWaveNormal(p, 0.25, normalize(vec2(1.4, 1.0)), 11.0, 2.4, u_Time, 1.5);
    sum += directionalWaveNormal(p, 0.125, normalize(vec2(-0.8, -1.0)), 10.0, 2.0, u_Time, 2.0);
    sum += directionalWaveNormal(p, 0.0625, normalize(vec2(1.3, 1.0)), 15.0, 4.0, u_Time, 2.2);
    sum += directionalWaveNormal(p, 0.03125, normalize(vec2(-1.7, -1.0)), 5.0, 1.8, u_Time, 3.0);
    return normalize(vec3(-sum.x, -sum.y, 1.0));
}

void outside(vec4 mask) {
    
    float dist = 5 * 5 * 4.0;

    vec2 p = (2.0 * gl_FragCoord.xy - u_Size) / min(u_Size.x, u_Size.y);

    vec3 normal = summedWaveNormal(p);

    vec3 c = mix(vec3(0.31, 0.15, 0.1), vec3(0.2, 0.25, 0.4), dot(normal, normalize(vec3(0.1, 0.2, 0.5))) * 0.5 + 0.5);
    c = mix(c, vec3(0.7, 0.9, 1.0), pow(dot(normal, normalize(vec3(-0.4, 0.1, 1.0))) * 0.5 + 0.5, 2.0));
    c = mix(c, vec3(0.9, 0.98, 1.0), pow(dot(normal, normalize(vec3(-0.1, -0.3, 0.5))) * 0.5 + 0.5, 10.0));
    
    if (u_Glow) {
            for (int x = -u_GlowSize; x <= u_GlowSize; x++) {
                for (int y = -u_GlowSize; y <= u_GlowSize; y++) {
                    vec4 neighbor = texture(u_Texture, v_TexCoord + v_OneTexel * vec2(x, y));
                    if (mask != neighbor) {
                        float ndist = x * x + y * y - 1.0;
                        dist = min(dist, ndist);
                    }
                }
            }
        }
    
        
    
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                vec4 neighbor = texture(u_Texture, v_TexCoord + v_OneTexel * vec2(x, y));
    
                if (mask != neighbor) {
                    if (u_UseEffect) {
                        FragColor = vec4(c, 1.0f);
                    } else {
                        FragColor = vec4(u_OutlineColor, 1.0f);
                    }
                    return;
                }
            }
        }
        
    
        float minDist = float(5 * 5);
        float glow = 1.0 - clamp(dist / minDist, 0.0, 1.0);
        if (!u_Glow)
            glow = 0.0;
    
        if (u_UseEffect) {
             FragColor = vec4(c, glow * 0.5);
        } else {
            FragColor = vec4(u_OutlineColor, glow * 0.5);
        }
}

void inside() {
    vec2 p = (2.0 * gl_FragCoord.xy - u_Size) / min(u_Size.x, u_Size.y);

	vec3 normal = summedWaveNormal(p);
	
	vec3 c = mix(vec3(0.31, 0.15, 0.1), vec3(0.2, 0.25, 0.4),  dot(normal, normalize(vec3(0.1, 0.2, 0.5))) * 0.5 + 0.5);
	c = mix(c, vec3(0.7, 0.9, 1.0), pow(dot(normal, normalize(vec3(-0.4, 0.1, 1.0))) * 0.5 + 0.5, 2.0));
	c = mix(c, vec3(0.9, 0.98, 1.0), pow(dot(normal, normalize(vec3(-0.1, -0.3, 0.5))) * 0.5 + 0.5, 10.0));
	
	if (u_UseEffect) {
        FragColor = vec4(c, 0.3);
    } else {
        FragColor = vec4(u_OutlineColor, 0.3);
    }
}



void main() {

    vec4 mask = texture(u_Texture, v_TexCoord);

    bool isMask = length(mask.rgb) > 0.1;
    if (isMask)
        inside();
    else
        outside(mask);

    
})";

