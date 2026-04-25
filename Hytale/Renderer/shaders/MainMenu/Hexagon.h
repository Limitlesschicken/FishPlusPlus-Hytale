/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

static constexpr const char* HEXAGON_FRAG = R"(#version 330 core

//https://www.shadertoy.com/view/fsdcz7

in vec2 v_TexCoord;
in vec2 v_OneTexel;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec2 u_Size;
uniform float u_Time;
uniform int u_FrameCount;

// Using a hexagon grid based pattern, instead of a square one.
#define HEXAGON

// Random cell subdivsion: Commenting this out would mean compulsory cell 
// subdivision. Whether you leave this in or not depends on the look you're after.
#define RANDOM_SUBDIVISION

// Thick outer cell borders for a more cartoonish look.
//#define THICK_BORDER
        

// Standard 2D rotation formula.
mat2 rot2(in float a){ float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }


// IQ's vec2 to float hash.
float hash21(vec2 p){  return fract(sin(dot(p, vec2(27.619, 57.583)))*43758.5453); }


float sBoxS(in vec2 p, in vec2 b, in float rf){
  
  vec2 d = abs(p) - b + rf;
  return min(max(d.x, d.y), 0.) + length(max(d, 0.)) - rf;
    
}

// Flat top or pointed top hexagon.
#define FLAT_TOP_HEXAGON
//
// Helper vector. If you're doing anything that involves regular triangles or hexagons, the
// 30-60-90 triangle will be involved in some way, which has sides of 1, sqrt(3) and 2.
#ifdef FLAT_TOP_HEXAGON
vec2 s = vec2(1.7320508, 1);
#else
vec2 s = vec2(1, 1.7320508);
#endif


// The 2D hexagonal isosuface function: If you were to render a horizontal line and one that
// slopes at 60 degrees, mirror, then combine them, you'd arrive at the following. As an aside,
// the function is a bound -- as opposed to a Euclidean distance representation, but either
// way, the result is hexagonal boundary lines.
float hex(in vec2 p){
    
    p = abs(p);
    
    #ifdef FLAT_TOP_HEXAGON
    // Below is equivalent to:
    //return max(p.x*.866025 + p.y*.5, p.y); 

    return max(dot(p, vec2(1.7320508, 1)*.5), p.y); // Hexagon.
    #else
    // Below is equivalent to:
    //return max(p.x*.5 + p.y*.866025, p.x); 

    return max(dot(p, vec2(1, 1.7320508)*.5), p.x); // Hexagon.
    #endif
    
}

// This function returns the hexagonal grid coordinate for the grid cell, and the corresponding 
// hexagon cell ID -- in the form of the central hexagonal point. That's basically all you need to 
// produce a hexagonal grid.
//
// When working with 2D, I guess it's not that important to streamline this particular function.
// However, if you need to raymarch a hexagonal grid, the number of operations tend to matter.
// This one has minimal setup, one "floor" call, a couple of "dot" calls, a ternary operator, etc.
// To use it to raymarch, you'd have to double up on everything -- in order to deal with 
// overlapping fields from neighboring cells, so the fewer operations the better.
vec4 getHex(vec2 p){
    
    // The hexagon centers: Two sets of repeat hexagons are required to fill in the space, and
    // the two sets are stored in a "vec4" in order to group some calculations together. The hexagon
    // center we'll eventually use will depend upon which is closest to the current point. Since 
    // the central hexagon point is unique, it doubles as the unique hexagon ID.
    #ifdef FLAT_TOP_HEXAGON
    vec4 hC = floor(vec4(p/s, p/s - vec2(1.7320508/3., .5))) + .5;
    #else
    vec4 hC = floor(vec4(p/s, p/s - vec2(.5, 1.7320508/3.))) + .5;
    #endif    
   
    // Centering the coordinates with the hexagon centers above.
    vec4 h = vec4(p - hC.xy*s, p - (hC.zw + .5)*s);
    //vec4 h = p.xyxy - vec4(hC.xy + .5, hC.zw)*s.xyxy;
    
    
    // Nearest hexagon center (with respect to p) to the current point. In other words, when
    // "h.xy" is zero, we're at the center. We're also returning the corresponding hexagon ID --
    // in the form of the hexagonal central point. By the way, the unique ID (the .zw bit), 
    // needs to be multiplied by "s" to give the correct quantized position back. 
    // For example: float ns = noise2D(hID*s);
    //
    // On a side note, I sometimes compare hex distances, but I noticed that Iomateron compared
    // the squared Euclidian version, which seems neater, so I've adopted that. 
    return dot(h.xy, h.xy)<dot(h.zw, h.zw) ? vec4(h.xy, hC.xy) : vec4(h.zw, hC.zw + .5);

}

// A second offset distance field value. Used for hilighting.
float dHi;

vec3 distField(vec2 p){

    // Set the distance (and highlight distance) to a minimum.
    float d = -1e5;
    dHi = -1e5;
    
    // Overal unique cell ID.
    vec2 gIP = vec2(0);
    
    // Inititalize the scale to one.
    float sc = 1.;
    
    // Directional light.
    vec2 ld = normalize(vec2(1, 1.5));
    
    // Rotate and translate the coordinates and light.
    p *= rot2(u_Time/8.);
    p -= u_Time/16.;
    ld *= rot2(u_Time/8.);
     
   
    // Six subdivided grid partitions.
    for(int i = 0; i<6; i++){
    
        
        // Get the hexagon (or square) grid information
        // (local coordinates and cell ID) for this iteration.
        #ifdef HEXAGON
        vec4 p4 = getHex(p);
        p = p4.xy;
        vec2 ip = p4.zw;
        d = max(d, hex(p) - .5/sc); // Hexagon distance.
        dHi = max(dHi, (hex(p + ld*.001) - .5/sc)); // Highlight distance.
        #else
        vec2 ip = floor(p*sc) + .5;
        p -= ip/sc;
        d = max(d, sBoxS(p, vec2(.5/sc), 0.)); // Square distance.
        dHi = max(dHi, sBoxS(p + ld*.001, vec2(.5/sc), 0.)); // Highlight distance.
        #endif
  
        
  
        // Rescale the grid for the next iteration.
        #ifdef HEXAGON
        sc *= 1.4;
        s /= 1.4;
        #else
        sc *= 1.5;
        #endif
        
        // Update the overall cell ID.
        gIP += ip/sc;    
        
        // Optional random rotation -- It looks more interesting, but
        // it's not mandatory.
        p *= rot2((hash21(gIP + .05) - .5)*6.2831);
        ld *= rot2((hash21(gIP + .05) - .5)*6.2831);
       
        // Random translation.
        p -= vec2(hash21(gIP + .13), hash21(gIP + .04))/sc*.75;
        
        #ifdef RANDOM_SUBDIVISION
        // Random cell subdivsion. Commenting this out would mean compulsory
        // cell subdivision. Whether you leave this in or not depends on what
        // look you're after.
        if(i>2 && hash21(gIP + .22)<.2) break;
        #endif
        
        /*
        // Internal moving rotation -- Interesting, but a bit much.
        float dir = hash21(gIP + .07)<.5? -1. : 1.;
        p *= rot2(dir*u_Time/sc/8.);
        ld *= rot2(dir*u_Time/sc/8.);
        */ 
        
        // Extra temporal translation. Also not mandatory.
        p -= u_Time/32.;
         
        
        
    
    }
   
    // Return the cell distance and unique ID.
    return vec3(d, gIP);
}

void main(){

    // Aspect corret coordinates.
    vec2 uv = (gl_FragCoord.xy - u_Size.xy*.5)/u_Size.y;

    // Scale and smoothing factor.
    const float sc = 1.;
    float sf = sc/u_Size.y;
    
    
    // Scaling and translation.
    vec2 p = sc*uv;
    
    // Scene object -- Returns distance and ID.
    vec3 d = distField(p); 
    
    // Rendering onto the canvas.
    
    // ID based cell coloring.
    float rnd = hash21(d.yz + .1);
    float taper = rnd<.35? .7 : 1.;
    vec3 oCol = .5 + .45*cos(6.2831*hash21(d.yz + .2)/5. + vec3(0, 1, 2)/taper);
    if(rnd>=.35) oCol = mix(oCol.zyx, vec3(1)*dot(oCol, vec3(.299, .587, .114)), .75)/4.;
    //else oCol = oCol.zyx;
    
    // Directional derivative bump map calculation for some highlighting.
    float b = max(dHi - d.x, 0.)/.001;
  
    oCol = oCol*(.25 + b*.75)*1.35;
   
     
    // Diagonal line pattern.
    //
    // Resolution independent line number -- Not PPI independent though.
    float lns = 120.*u_Size.y/450.;
    vec2 rp = rot2(-3.14159/3.)*p;
    float pat = abs(fract((rp.x)*lns) - .5)*2. - .05;
    pat = smoothstep(0., sf*lns*2., pat); 
    
    float ew = .005*450./u_Size.y; // Resolution independent edge width.
    
    // Scene color.
    vec3 col = oCol*(pat*1. + .5);
    
    // Cell border.
    #ifdef THICK_BORDER
    const float bw = .0025; // Border width.
    #else
    const float bw = .001; // Border width.
    #endif
    // Border distance.
    float dBord = abs(d.x + bw) - bw; 
    
    // Application... There are definitely better ways, but I was pushed for time.
    col = mix(col, vec3(0), (1. - smoothstep(0., sf*8., dBord - ew))*.25); // Inner gradient.
    col = mix(col, vec3(0), (1. - smoothstep(0., sf, dBord - ew - .005))*.8); // Inner edge.
    col = mix(col, mix(oCol, vec3(1), .125), 1. - smoothstep(0., sf, dBord - ew)); // Colored edge.
    col = mix(col, vec3(0), 1. - smoothstep(0., sf, dBord)); // Outer edge.
    

    // Vertical color gradient of sorts.
    float grad = clamp(uv.y + .65, 0., 1.);
    col = mix(col.yxz, col.zyx, grad*grad);
    
    // Red to blue colors.
    //col = col.yxz;
    
    // Rough gamma correction and screen presentation.
    FragColor = vec4(sqrt(max(col, 0.)), 1);
    
})";