/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

static constexpr const char* MOBIUS_SPHERE_FRAG = R"(#version 330 core

//https://www.shadertoy.com/view/7fl3DX

in vec2 v_TexCoord;
in vec2 v_OneTexel;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec2 u_Size;
uniform float u_Time;
uniform int u_FrameCount;

// Cotterzz's raytraced sphere fix: The standard function most 
// of us use doesn't really cater for miniscule spheres. If speed
// was a concern and the spheres were larger (most of the time,
// they are), you could use the regular one.
float traceSphere(in vec3 ro, in vec3 rd, in vec4 sph){

    vec3 oc = ro - sph.xyz;
	float b = dot(oc, rd);
    if(b>0.) return 1e8;
    // OLD: catastrophic cancellation near silhouette edges
    // float c = dot(oc, oc) - sph.w*sph.w;
    // float h = b*b - c;

    // NEW: h = r² - |oc × rd|²  (stable, no large-minus-large)
    vec3 cx = cross(oc, rd);
    float h = sph.w*sph.w - dot(cx, cx);

    if(h < 0.) return 1e8;
    return -b - sqrt(h);    
}
 
// Plane intersection: Old formula, and could do with some tidying up.
// The tiny "9e-7" figure is something I hacked in to stop near plane 
// artifacts from appearing. I don't like it at all, but not a single 
// formula I found deals with the problem. There definitely has to be
// a better way, so if someone knows of a more robust formula, I'd 
// love to use it.
float tracePlane(vec3 ro, vec3 rd, vec3 n, vec3 o){


    float t = 1e8;
 
	float ndotdir = dot(rd, n);
     
	if (ndotdir<0.){
	
		float dist = -(dot(ro - o, n) + 9e-7*0.)/ndotdir;	// + 9e-7
   		
		if (dist>0.){ 
            t = dist; 
  		}
	}
    
    return t;

}

// This example only works with a flat top arrangement, but
// I'll arrange for it to work with both later.
#define FLAT_TOP_HEXAGON

// Helper vector. If you're doing anything that involves regular triangles or hexagons, the
// 30-60-90 triangle will be involved in some way, which has sides of 1, sqrt(3) and 2.
#ifdef FLAT_TOP_HEXAGON
const vec2 s = vec2(1.7320508, 1)/2.;
#else
const vec2 s = vec2(1, 1.7320508)/2.;
#endif



float hex(in vec2 p){
    
    p = abs(p);
    
    #ifdef FLAT_TOP_HEXAGON
    // Below is equivalent to:
    //return max(p.x*.866025 + p.y*.5, p.y); 

    return max(dot(p, vec2(1.7320508, 1)/2.), p.y); // Hexagon.
    #else
    // Below is equivalent to:
    //return max(p.x*.5 + p.y*.866025, p.x); 

    return max(dot(p, vec2(1, 1.7320508)/2.), p.x); // Hexagon.
    #endif
    
}

vec4 getHex(vec2 p){

    vec4 h = vec4(p, p - s/2.);
    // Their respective IDs. iC*s.xyxy represent the cell centers.
    vec4 iC = floor(h/s.xyxy) + .5;
     
    // Centering the coordinates with hexagon centers above to
    // produce respective local coordinates.
    h -= iC*s.xyxy; 
 
    // Determine the nearest hexagon cell, then return the local coordinates
    // and the integer IDs. Multiplying the ID by "s" will give you the
    // position based hexagon center.
    return dot(h.xy, h.xy)<dot(h.zw, h.zw)? vec4(h.xy, iC.xy) : vec4(h.zw, iC.zw + .5);     

}


///// Variable defines.

// The grid element. The circles almost look like a mapped
// spherical Doyle spiral, but not quite... I have one of 
// those that I'll post later.
//
// Grid shape: Hexagons: 0, Circles: 1.
#define SHAPE 0
   
// Display the sphere.
#define SHOW_SPHERE
 
////////////////////


#define PI 3.14159265358979323846
#define TAU 6.28318530717958647693

// Max ray distance.
#define FAR 20.


// Scene object ID.
int objID;


// Standard 2D rotation formula.
mat2 rot2(in float a){ float c = cos(a), s = sin(a); return mat2(c, s, -s, c); }

// Hash without Sine -- Dave Hoskins
// https://www.shadertoy.com/view/4djSRW
// 1 out, 2 in...
float hash21(vec2 p) {
 
    p = fract(p*vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x*p.y);
}

// IQ's box function.
float sBoxS(in vec2 p, in vec2 b, in float rf){
  
  vec2 d = abs(p) - b + rf;
  return min(max(d.x, d.y), 0.) + length(max(d, 0.)) - rf;
    
}

 
// Object distance holder. We're only using two, so this
// is a bit of overkill.
vec4 vObj;

// The extruded image.
float map(vec3 p){
    
    // Floor.
    float fl = p.y + .5;
     
    // Sphere.
    #ifdef SHOW_SPHERE
    float sph = length(p) - .5;
    #else
    float sph = 1e5;
    #endif
      
       
    // Object IDs.
    vObj = vec4(fl, sph, 1e5, 1e5);
    
    // Minimum scene distance.
    return min(fl, sph); 
 
}

 
// Basic raymarcher.
float trace(in vec3 ro, in vec3 rd){

    // Overall ray distance and scene distance.
    float t = 0., d;
    
    for(int i = min(0, u_FrameCount); i<128; i++){
    
        d = map(ro + rd*t);
        if((abs(d)<.001 && d<0.) || t>FAR) break;
        
        t += d*.9; 
    }

    return min(t, FAR);
}


// Standard normal function. It's not as fast as the tetrahedral calculation, 
// but more symmetrical.
vec3 getNormal(in vec3 p, float t) {
	const vec2 e = vec2(.001, 0);
	return normalize(vec3(map(p + e.xyy) - map(p - e.xyy), map(p + e.yxy) - map(p - e.yxy),	
                          map(p + e.yyx) - map(p - e.yyx)));
}


// Cheap shadows are hard. In fact, I'd almost say, shadowing particular scenes with
// limited iterations is impossible... However, I'd be very grateful if someone could 
// prove me wrong. :)
float softShadow(vec3 ro, vec3 lp, vec3 n, float k){

    // More would be nicer. More is always nicer, but not always affordable. :)
    const int maxIterationsShad = 32; 
    
    ro += n*.0015; // Coincides with the hit condition in the "trace" function.  
    vec3 rd = lp - ro; // Unnormalized direction ray.

    float shade = 1.;
    float t = 0.; 
    float end = max(length(rd), .0001);
    //float stepDist = end/float(maxIterationsShad);
    rd /= end;

    // Max shadow iterations - More iterations make nicer shadows, but slow things down. 
    // Obviously, the lowest number to give a decent shadow is the best one to choose. 
    for (int i = min(u_FrameCount, 0); i<maxIterationsShad; i++){

        float d = map(ro + rd*t);
        shade = min(shade, k*d/t);
        //shade = min(shade, smoothstep(0., 1., k*d/t)); // Thanks to IQ for this tidbit.
        // So many options here, and none are perfect: dist += clamp(d, .01, stepDist), etc.
        t += clamp(d, .005, .15); 
        
        
        // Early exits from accumulative distance function calls tend to be a good thing.
        if (d<0. || t>end) break; 
    }

    // Cap above zero.
    return max(shade, 0.); 
}


// I keep a collection of occlusion routines... OK, that sounded really nerdy. :)
// Anyway, I like this one. I'm assuming it's based on IQ's original.
float calcAO(in vec3 p, in vec3 n){

	float sca = 2., occ = 0.;
    for( int i = 0; i<5; i++ ){
    
        float hr = float(i + 1)*.15/5.;        
        float d = map(p + n*hr);
        occ += (hr - d)*sca;
        sca *= .7;
    }
    
    return clamp(1. - occ, 0., 1.);  
}

// Standard complex transform functions. 
vec2 cmul(vec2 a, vec2 b){ return mat2(a, -a.y, a.x)*b; }
vec2 cinv(vec2 a){ return vec2(a.x, -a.y)/dot(a, a); }
vec2 cdiv(vec2 a, vec2 b){ return cmul(a, cinv(b)); }
vec2 clog(in vec2 z){ return vec2(log(length(z)), atan(z.y, z.x)); }
vec2 cexp(vec2 z){ return exp(z.x)*vec2(cos(z.y), sin(z.y)); }
vec2 cpow(vec2 a, vec2 b){ return cexp(cmul(b, clog(a))); }


// Rolling the ball on the plane.
vec3 rollObj(vec3 p){

    //p.xy *= rot2(u_Time/4.);
    p.xz *= rot2(u_Time/2.);
    p.yz *= rot2(u_Time/1.);
    return p;
}

 

// Stereographically maps the unit sphere sittin on a plane
// to the plane itself.
vec2 stereographic(vec3 p){
 
    return p.xz / (1. - p.y);
}

// Reverse stereographic mapping of the plane to a unit sphere
// sitting on it.
vec3 stereographicInverse(vec2 p) {

    float r2 = dot(p, p);
    return vec3(2. * p.x, r2 - 1., 2. * p.y) / (1. + r2);
}



// Floor. 
vec2 fFloor(vec3 p) {

    // Floor.    

    // Move the pattern to the ball, rotate it, then 
    // project it back down to the floor.    
    p = stereographicInverse(p.xz);
    p = rollObj(p);
    return stereographic(p);

}

// Spiral repetition factor. Positive integers will work.
vec2 rep = vec2(4, 8);

vec3 transform(vec3 p) {

    // If there were no rolling movement, you wouldn't need this.
    p.xz = fFloor(p);


    // 2D plane transformations.

    // The three lines below will produce a standard Mobius spiral
    // pattern. Commenting in the other two lines will produce something
    // more interesting, but I figured it was a bit much for this 
    // example.
    //p.xz = cpow(p.xz - vec2(1, .5), vec2(3., 0));   

#if 1
// Mobius transformation -- Probably the simplest one.
    p.xz = cdiv(p.xz - vec2(1, 0), p.xz + vec2(1, 0));
    // p.xz += cdiv(p.xz - vec2(.5, 0).yx, p.xz + vec2(.5, 0).yx);
    // This turns the Mobius transform (above) into a double spiral.
    p.xz = clog(p.xz);
#else
// Something more interesting -- Cool looking, but a bit much for this example.
    float N = 2.; // N-tuples of spirals... 3, 4, etc.
    p.xz = clog(cdiv(vec2(2, 0), cpow(p.xz, vec2(N, 0)) - vec2(1, 0)) + vec2(1, 0));
#endif

    // The "rep" factor controls how the spiral looks.
    p.xz = cmul(p.xz, rep * vec2(1, sqrt(3.) / 2.) / TAU);


    return p;

}


// Numeric transform function derivative. You could determine this analytically.
// In fact, I usually do, but this allows you try out more interesting complex
// transform combinations.
vec3 funcD(vec3 p) {

    // Numeric derivative.
    float px = 1e-4;
    vec3 f = transform(p);
    vec3 dtX = (transform(p + vec3(px, 0, 0)) - f) / px;
    vec3 dtY = (transform(p + vec3(0, px, 0)) - f) / px;
    vec3 dtZ = (transform(p + vec3(0, 0, px)) - f) / px;
    //return vec3(length(dtX), length(dtY), length(dtZ))/sqrt(3.);
    return (mat3(dtX, dtY, dtZ) * vec3(1)) / sqrt(3.);

}

// Decorative circle distances... Added in at the last minutes.
float gCir;
float gCir2;

vec3 getPattern(vec3 p3) {

    // Pattern scale.
    vec2 sc = vec2(1) / 2.;

    // Transform derivative. Used to produce border lines of equal
    // width... Not technically correct, but close enough for this example.
    float tdF = length(funcD(p3));

    // Perform the complex transform. In this case, a
    // Mobius spiral.
    p3 = transform(p3);

    // Floor (2D grid) coordinates.
    vec2 p = p3.xz;

    // Produce the hexagon grid.
    vec4 p4 = getHex(p);
    // Local coordiantes and ID.
    p = p4.xy;
    vec2 ip = p4.zw;


    // Grid shape: Circles or hexagons.
#if SHAPE == 1
// Circles.
    float poly = length(p) - s.y / 2.;
#else 
    float poly = hex(p) - s.y / 2.;
#endif


    // Cut out a dot in the center.
    poly = max(poly, -(length(p) - sc.x / 32.));

    // Divide by the transform derivative. This will give you equal
    // cell border widths, in a lot of cases.
    poly /= tdF;


    // Last minute circle functions, to use for decorative
    // purposes. Not really necessary.
    vec2 offs = s * vec2(1, -1); // Offsets.
    vec2 offs2 = s * vec2(1, -1) / 8.;

    gCir = -(length(p - offs2) - s.y / 2. / .866);
    gCir /= tdF;

    gCir2 = length(p - offs2.yx / 2.) - sc.x / 5.;
    gCir2 /= tdF;


    // Cell distance and cell ID.
    return vec3(poly, ip);
}

void main() {

    vec2 uv = (gl_FragCoord.xy - u_Size * .5) / u_Size.y;

    vec3 ro = vec3(0, 1.5, -2); // Camera position, doubling as the ray origin.
    vec3 lk = vec3(0, -.25, 0);//vec3(0, -.25, u_Time);  // "Look At" position.

    vec3 lp = ro + vec3(.75, 0, 1.5);// Put it a bit in front of the camera.

    float FOV = .85; // FOV - Field of view.
    vec3 fwd = normalize(lk - ro); // Forward.
    vec3 rgt = normalize(cross(vec3(0, 1, 0), fwd));// Right. 
    vec3 up = cross(fwd, rgt); // Up.

    vec3 rd = normalize(uv.x * rgt + uv.y * up + fwd / FOV);


    float t = trace(ro, rd);

    float minDist = 1e5;

    objID = vObj.x < vObj.y ? 0 : 1;

    vec3 col = vec3(0);

    if (t < FAR) {


        vec3 sp = ro + rd * t;
        vec3 sn = getNormal(sp, t);

        vec3 ld = lp - sp;

        float lDist = max(length(ld), .001);

        ld /= lDist;

        float sh = softShadow(sp, lp, sn, 16.);
        float ao = calcAO(sp, sn); // Ambient occlusion.

        float atten = 1. / (1. + lDist * .05);

        float diff = max(dot(sn, ld), 0.);

        float spec = pow(max(dot(reflect(ld, sn), rd), 0.), 32.);


        float Schlick = pow(1. - max(dot(rd, normalize(rd + ld)), 0.), 5.);
        float freS = mix(.15, 1., Schlick);  //F0 = .2 - Glass... or close enough. 


        // Obtaining the texel color. 
        vec3 texCol = vec3(.6);

        // Texture coordinates.
        vec3 txP = sp;

        // Smooth factor and edge width.
        float sf = .005;
        float ew = .018;

        // Pattern variable: Distance and ID.
        vec3 d3;

        // Object coloring.
        if (objID > 0) {

            // Ball. 

            ////
            // Raytrace from the ball to the floor, then obtain the 
            // Mobius spiral pattern value.

            // North pole to hit point unit direction vector.
            vec3 rd2 = normalize(txP - vec3(0, .5, 0));
            // Use it to trace from the hit point to the plane.
            float t2 = tracePlane(txP, rd2, vec3(0, 1, 0), vec3(0, -.5, 0));
            // Intersection point on the plane.
            vec3 txP2 = txP + rd2 * t2;
            // Obtain the pattern values for coloring: Distance and ID.
            d3 = getPattern(txP2);
            ////

            // Applying a hack to approximate equi-spaced lines... I was in a 
            // hurry... I haven't decided whether a precise calculation is possible.
            // Either way, this looks OK for the purpose of this demonstration.
            ew *= 2. / length(txP - vec3(0, 1, 0));
            //ew *= 2.*pow(1. + txP.y, 2.);

        }
        else {

            // Floor.
            d3 = getPattern(txP);

        }

        // Wrapping the cell ID with the Mobius spiral repetition factor.
        vec2 id = mod(d3.yz, rep.yx) / (rep.x * rep.y);

        // Rand, or non-random ID value.
        float rnd = hash21(id + .1);
        //rnd = (d3.y + d3.z*4.)/(4.*4.);
        //rnd = id.y + id.x*rep.y;
        //rnd = id.x*rep.x + id.y*rep.y;
        rnd = dot(id, rep);

        // Use the ID to color the cell in some way.
        // Two slightly different shades.
        vec3 cCol = .5 + .45 * cos(TAU * rnd + vec3(0, PI / 2., PI));
        vec3 cCol2 = .5 + .45 * cos(TAU * rnd + .25 + vec3(0, PI / 2., PI));
        // Order the two shades according to brightness.
        if (dot(cCol2 - cCol, vec3(299, .587, .114)) < 0.) {
            vec3 tmp = cCol; cCol = cCol2; cCol2 = tmp;
        }
        // Render.
        cCol = mix(cCol, cCol2 * 1.2 + .05, 1. - smoothstep(0., sf * 2., gCir));
        cCol = mix(cCol, cCol * 1.5 + .1,
            1. - smoothstep(0., sf * 2., abs(gCir + ew / 4.) - ew / 4.));


        // Background.
        texCol = vec3(.0);
        // Cell color.
        texCol = mix(texCol, cCol, 1. - smoothstep(0., sf, d3.x + ew));


        // Rendering an edge ring around the sphere.
        //
        // Adapted from one of IQ's world to screen space examples. 
        // I'll track down the particular one later.
#ifdef SHOW_SPHERE
        mat3 cam = mat3(rgt, up, fwd);
        mat4 cam4 = mat4(rgt, 0, up, 0, fwd, 0, ro, 1.);
        mat4 invCam = inverse(cam4);
        vec3 qq = (invCam * vec4(vec3(0), 1.)).xyz;
        // 2D Screen space.
        vec2 s = (uv - qq.xy / qq.z / FOV) * qq.z;
        float r = .5 / FOV;
        texCol = mix(texCol, vec3(0),
            1. - smoothstep(0., .003, abs(length(s) - r - .015) - .01));
#endif



        // Combining the above terms to produce the final color.
        col = texCol * (diff * sh + .3 + vec3(1, .97, .92) * spec * freS * 2. * sh);

        // Extra light.
        //col += col/(1. + lDist*lDist);

        // Shading.
        col *= ao * atten;


    }


    // Horizon fog. Not visible here, but provided for completeness.
    col = mix(col, vec3(0), smoothstep(0., .99, t / FAR));


    // Subtle vignette. Designers use them to frame things and guide
    // the viewer's eyes toward the center... or something like that.
    vec2 w = vec2(u_Size.x / u_Size.y, 1);
    col *= 1.05 - smoothstep(0., .1, sBoxS(uv, w / 2., .15) + .1) * .15;


    // Rought gamma correction.
    FragColor = vec4(sqrt(max(col, 0.)), 1);

}
)";